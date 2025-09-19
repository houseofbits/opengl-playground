#pragma once
#include <atomic>

template<class T>
class DoubleBuffer {
public:
    T &get() {
        int readIndex = m_currentBufferIndex.load(std::memory_order_acquire);
        return m_bufferedValue[readIndex];
    }

    void set(const T &value) {
        const int writeIndex = 1 - m_currentBufferIndex.load(std::memory_order_relaxed);
        m_bufferedValue[writeIndex] = value;
        m_currentBufferIndex.store(writeIndex, std::memory_order_release);
    }

private:
    std::atomic<int> m_currentBufferIndex{0};
    T m_bufferedValue[2];
};
