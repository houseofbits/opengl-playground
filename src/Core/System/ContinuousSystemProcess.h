#pragma once
#include <atomic>
#include <list>
#include <thread>

#include "BaseSystemProcess.h"

class ContinuousSystemProcess final : public BaseSystemProcess {
public:
    explicit ContinuousSystemProcess(EventManager &eventManager, long frequencyMs);

    void process() override;

private:
    std::thread m_thread;
    std::atomic<bool> m_processRunning;
    long m_frequencyMs;
};
