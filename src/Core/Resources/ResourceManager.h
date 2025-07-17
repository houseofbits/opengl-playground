#pragma once

#include "../../Core/Helper/Log.h"
#include "../Reflection/Type.h"
#include "Resource.h"
#include <algorithm>
#include <functional>
#include <atomic>
#include <cstdio>
#include <ctime>
#include <list>
#include <thread>
#include <utility>
#include <vector>

class ResourceManager {
public:
    ResourceManager() : m_Resources(), m_FetchThread(&ResourceManager::fetchProcess, this),
                        m_FetchProcessRunning(true) {
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
        return std::any_of(m_Resources.begin(), m_Resources.end(), [&path](Resource *resource) {
            return resource->m_Path == path;
        });
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

    template<class T>
    std::vector<T *> getAllOfType() const {
        std::vector<T *> vec;
        for (auto resource: m_Resources) {
            if (isOfType<T>(resource)) {
                vec.push_back(static_cast<T *>(resource));
            }
        }

        return vec;
    }

    /**
     * @tparam T Instance of ResourceHandle
     * @param hand Resource handle
     * @param resourceName Path to the resource
     * @param dependencies Array of resource paths. Resource will only be loaded when all the dependencies are READY
     */
    template<class T>
    void request(T &hand, std::string resourceName, const std::vector<std::string> &dependencies = {}) {
        requestWith(hand, resourceName, [dependencies](typename T::TYPE &resource) {
            if (!dependencies.empty()) {
                resource.m_Dependencies.insert(resource.m_Dependencies.end(), dependencies.begin(),
                                               dependencies.end());
            }
        });
    }

    template<class T>
    void requestWith(T &hand, std::string resourceName, std::function<void(typename T::TYPE &)> fetchFunctor) {
        if (resourceName.empty()) {
            // Log::error("requestWith: Empty resource name");
            return;
        }

        Resource *resource = findResourceOfType<typename T::TYPE>(resourceName);
        if (resource == nullptr) {
            resource = new typename T::TYPE();
            resource->m_Path = std::move(resourceName);
            resource->m_Status = Resource::STATUS_DATA_FETCHING;

            fetchFunctor(*static_cast<typename T::TYPE *>(resource));

            m_Resources.push_back(resource);
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

    void buildFetchedResources() const {
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
        //        const auto timestep = std::chrono::milliseconds(100);
        while (true) {
            if (!m_FetchProcessRunning) {
                return;
            }
            auto start = std::chrono::steady_clock::now();
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

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();

            if (elapsed < 100) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100 - elapsed));
            }
            //            std::this_thread::sleep_for(timestep);
        }
    }

    bool areDependenciesReady(Resource *resource) const {
        // Log::write("Check resource ", resource->m_Path);
        if (!resource->m_Dependencies.empty()) {
            for (const auto &dependencyPath: resource->m_Dependencies) {
                const Resource *r = findResource(dependencyPath);
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
