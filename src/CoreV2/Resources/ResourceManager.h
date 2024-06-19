#pragma once

#include "../../Helper/Log.h"
#include "Resource.h"
#include <atomic>
#include <list>
#include <thread>
#include <utility>

class ResourceManager {
public:
    ResourceManager() : m_FetchThread(&ResourceManager::fetchProcess, this), m_FetchProcessRunning(true) {
    }
    ~ResourceManager() {
        m_FetchProcessRunning = false;
        m_FetchThread.join();
        for (const auto &resource: m_Resources) {
            remove(resource);
        }
    }

    template<class T>
    void request(T &hand, std::string path) {
        Resource *resource = new typename T::TYPE();
        resource->m_Path = std::move(path);
        resource->m_Status = Resource::DATA_FETCHING;

        m_Resources.push_back(resource);

        hand.makeValid(this, reinterpret_cast<typename T::TYPE *>(resource));
    }

    void remove(Resource *resource) {
        m_Resources.remove(resource);
        resource->destroy();
        delete resource;
    }

    void buildFetchedResources() {
        for (const auto &resource: m_Resources) {
            if (resource->m_Status == Resource::DATA_READY) {
                resource->build();
            }
        }
    }

    void fetchProcess() const {
        const auto wait_duration = std::chrono::milliseconds(100);
        while (true) {
            if (!m_FetchProcessRunning) {
                return;
            }
            for (const auto &resource: m_Resources) {
                if (resource->m_Status == Resource::DATA_FETCHING) {
                    resource->fetchData();
                }
            }

            std::this_thread::sleep_for(wait_duration);
        }
    }

    std::list<Resource *> m_Resources;
    std::thread m_FetchThread;
    std::atomic<bool> m_FetchProcessRunning;
};
