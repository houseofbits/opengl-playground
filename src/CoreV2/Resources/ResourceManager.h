#pragma once

#include "../../Helper/Log.h"
#include "Resource.h"
#include <algorithm>
#include <atomic>
#include <list>
#include <thread>
#include <utility>

class ResourceManager {
public:
    ResourceManager() : m_Resources(), m_FetchThread(&ResourceManager::fetchProcess, this), m_FetchProcessRunning(true) {
    }
    ~ResourceManager() {
        m_FetchProcessRunning = false;
        m_FetchThread.join();
        for (const auto &resource: m_Resources) {
            resource->destroy();
            delete resource;
        }
        m_Resources.clear();
    }

    bool doesResourceExist(const std::string &path) {
        return std::any_of(m_Resources.begin(), m_Resources.end(), [&path](Resource *resource) { return resource->m_Path == path; });
    }

    [[nodiscard]] Resource *findResource(const std::string &path) const {
        for (const auto &resource: m_Resources) {
            if (resource->m_Path == path) {
                return resource;
            }
        }

        return nullptr;
    }

    template<class T>
    void request(T &hand, std::string path) {
        Resource *resource = findResource(path);

        if (resource == nullptr) {
            Log::info("Fetch resource: " + path);
            resource = new typename T::TYPE();
            resource->m_Path = std::move(path);
            resource->m_Status = Resource::DATA_FETCHING;

            m_Resources.push_back(resource);
        } else {
            Log::info("Get resource: " + path);
        }

        hand.makeValid(this, reinterpret_cast<typename T::TYPE *>(resource));
    }

    void remove(Resource *resource) {
        //TODO: Add to list of removable entities and process at some other point
        m_Resources.remove(resource);
        resource->destroy();
        delete resource;
    }

    void buildFetchedResources() {
        for (const auto &resource: m_Resources) {
            if (resource->m_Status == Resource::DATA_READY) {
                resource->build();
                if (resource->m_Status == Resource::BUILD_ERROR) {
                    Log::warn("Failed to build resource: " + resource->m_Path);
                }
            }
        }
    }

    void fetchProcess() {
        const auto wait_duration = std::chrono::milliseconds(100);
        while (true) {
            if (!m_FetchProcessRunning) {
                return;
            }
            for (const auto &resource: m_Resources) {
                if (resource->m_Status == Resource::DATA_FETCHING) {
                    resource->fetchData(*this);

                    if (resource->m_Status == Resource::FETCH_ERROR) {
                        Log::warn("Failed to fetch resource: " + resource->m_Path);
                    }
                }
            }

            std::this_thread::sleep_for(wait_duration);
        }
    }

    std::list<Resource *> m_Resources;
    std::thread m_FetchThread;
    std::atomic<bool> m_FetchProcessRunning;
};
