#pragma once

#include "ResourceManager.h"

class ResourceManager;

template<class T>
class ResourceHandle {
public:
    ResourceHandle() : m_ResourceManager(nullptr) {}
    ~ResourceHandle() {
        invalidate();
    }

    typedef T TYPE;

    bool isValid() {
        return m_ResourceManager != nullptr && m_Resource != nullptr;
    }

    T &operator()() {
        if (!isValid()) {
            createDefaultInstance();
            return *reinterpret_cast<T *>(T::m_DefaultInstance);
        }

        return *m_Resource;
    }

    void invalidate() {
        if (isValid()) {
            m_Resource->m_ReferenceCount--;
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
        m_Resource->m_ReferenceCount++;
    }

    void createDefaultInstance() {
        if (m_Resource->m_DefaultInstance == nullptr) {
            m_Resource->m_DefaultInstance = new T();
        }
    }

    T *m_Resource;
    ResourceManager *m_ResourceManager;
};
