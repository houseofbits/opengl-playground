#pragma once

#include "ResourceManager.h"

class ResourceManager;

template<class T>
class ResourceHandle {
public:
    ResourceHandle() : m_Resource(nullptr), m_ResourceManager(nullptr) {
    }

    ~ResourceHandle() {
        invalidate();
    }

    typedef T TYPE;

    bool isValid() const {
        return m_ResourceManager != nullptr && m_Resource != nullptr;
    }

    [[nodiscard]] bool isReady() const {
        return isValid() && m_Resource->isReady();
    }

    T &get() {
        if (!isValid()) {
            createDefaultInstance();
            return *reinterpret_cast<T *>(T::m_DefaultInstance);
        }

        return *m_Resource;
    }

    T &operator()() {
        return get();
    }

    void invalidate() {
        if (isValid()) {
            --m_Resource->m_ReferenceCount;
            if (m_Resource->m_ReferenceCount <= 0) {
                m_ResourceManager->remove(m_Resource);
            }

            m_ResourceManager = nullptr;
            m_Resource = nullptr;
        }
    }

    void makeValid(ResourceManager *m, T *r) {
        invalidate();

        m_ResourceManager = m;
        m_Resource = r;
        ++m_Resource->m_ReferenceCount;
    }

    void createDefaultInstance() {
        if (m_Resource->m_DefaultInstance == nullptr) {
            m_Resource->m_DefaultInstance = new T();
        }
    }

    void from(ResourceHandle<T>& handle) {
        if (!handle.isValid()) {
            return;
        }

        if (isValid()) {
            invalidate();
        }

        makeValid(handle.m_ResourceManager, handle.m_Resource);
    }

    T *m_Resource;
    ResourceManager *m_ResourceManager;
};
