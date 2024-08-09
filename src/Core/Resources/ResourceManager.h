#pragma once

#include "../../Core/Helper/Log.h"
#include "../Reflection/Type.h"
#include "Resource.h"
#include <algorithm>
#include <atomic>
#include <list>
#include <thread>
#include <utility>
#include <vector>

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
    [[nodiscard]] Resource *findResourceOfType(const std::string &path) const {
        for (auto resource: m_Resources) {
            if (resource->m_Path == path && isOfType<T>(resource)) {
                return resource;
            }
        }

        return nullptr;
    }

    /**
     * @tparam T Instance of ResourceHandle
     * @param hand Resource handle
     * @param path Path to the resource
     * @param dependencies Array of resource paths. Resource will only be loaded when all of the dependencies are READY
     */
    template<class T>
    void request(T &hand, std::string path, std::vector<std::string> dependencies = {}) {
        if (path.empty()) {
            return;
        }

        Resource *resource = findResourceOfType<typename T::TYPE>(path);
        if (resource == nullptr) {
            //            Log::info("Fetch resource: " + path);
            resource = new typename T::TYPE();
            resource->m_Path = std::move(path);
            resource->m_Status = Resource::STATUS_DATA_FETCHING;
            resource->m_Dependencies = std::move(dependencies);

            m_Resources.push_back(resource);
        } else {
            if (dependencies != resource->m_Dependencies) {
                Log::warn("Mismatch in dependency list for: " + path);
                return;
            }
            //            Log::info("Get resource: " + path);
        }

        hand.makeValid(this, reinterpret_cast<typename T::TYPE *>(resource));
    }

    void remove(Resource *resource) {
//        std::cout<<"remove "<<resource->m_Path<<std::endl;
        //TODO: Add to list of removable entities and process at some other point
        m_Resources.remove(resource);
        resource->destroy();
        delete resource;
    }

    void buildFetchedResources() {
        for (const auto &resource: m_Resources) {
            if (resource->m_Status == Resource::STATUS_DATA_READY) {
                if (areDependenciesReady(resource)) {
                    resource->m_Status = resource->build();
                    if (resource->m_Status == Resource::STATUS_BUILD_ERROR) {
                        Log::warn("Failed to build resource: " + resource->m_Path);
                    }
                } else if (doesDependencyHaveError(resource)) {
                    resource->m_Status = Resource::STATUS_BUILD_ERROR;
                    Log::warn("Failed to build resource: " + resource->m_Path + " dependency failed");
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
                if (resource->m_Status == Resource::STATUS_DATA_FETCHING) {
                    if (areDependenciesReady(resource)) {
                        resource->m_Status = resource->fetchData(*this);
                        if (resource->m_Status != Resource::STATUS_DATA_READY) {
                            Log::warn("Failed to fetch resource: " + resource->m_Path);
                        }
                    } else if (doesDependencyHaveError(resource)) {
                        resource->m_Status = Resource::STATUS_FETCH_ERROR;
                        Log::warn("Failed to fetch resource: " + resource->m_Path + " dependency failed");
                    }
                }
            }

            std::this_thread::sleep_for(wait_duration);
        }
    }

    bool areDependenciesReady(Resource *resource) const {
        if (!resource->m_Dependencies.empty()) {
            for (const auto &dependencyPath: resource->m_Dependencies) {
                Resource *r = findResource(dependencyPath);
                if (r == nullptr) {
                    return false;
                }
                if (!r->isReady()) {
                    return false;
                }
            }
        }

        return true;
    }
    bool doesDependencyHaveError(Resource *resource) const {
        if (!resource->m_Dependencies.empty()) {
            for (const auto &dependencyPath: resource->m_Dependencies) {
                Resource *r = findResource(dependencyPath);
                if (r == nullptr) {
                    return false;
                }
                if (r->m_Status == Resource::STATUS_FETCH_ERROR ||
                    r->m_Status == Resource::STATUS_BUILD_ERROR) {
                    return true;
                }
            }
        }

        return false;
    }

    std::list<Resource *> m_Resources;
    std::thread m_FetchThread;
    std::atomic<bool> m_FetchProcessRunning;
};
