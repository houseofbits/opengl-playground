#pragma once

#include "../../Core/Helper/Log.h"
#include "../Reflection/Type.h"
#include "Resource.h"
#include <algorithm>
#include <functional>
#include <atomic>
#include <cstdio>
#include <list>
#include <thread>
#include <utility>
#include <vector>
#include "ResourceFactory.h"

class ResourceManager {
public:
    ResourceManager() : m_Resources()
    {
    }

    ~ResourceManager() {
        for (const auto &resource: m_Resources) {
            resource->destroy();
            delete resource;
        }
        m_Resources.clear();
    }

    bool doesResourceExist(const std::string &path) {
        return std::any_of(m_Resources.begin(), m_Resources.end(), [&path](const Resource *resource) {
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

    template<class T>
    std::vector<T *> getAllReadyOfType() const {
        std::vector<T *> vec;
        for (auto resource: m_Resources) {
            if (resource->isReady() && isOfType<T>(resource)) {
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

    /**
    * @desciption Request resource of some abstract type T, by providing concrete resource typeName
    */
    template<class T>
    void requestAbstract(T &handle, const std::string &typeName, const std::string &resourceName,
                         const std::vector<std::string> &dependencies = {}) {
        if (resourceName.empty() || typeName.empty()) {
            return;
        }

        Resource *resource = findResourceOfType<typename T::TYPE>(resourceName);
        if (resource == nullptr) {
            resource = resourceFactory.createInstance(typeName);
            if (!resource) {
                Log::warn("Type not found in resource factory: ", typeName, ", while requesting resource: ",
                          resourceName);

                return;
            }
            if (!dependencies.empty()) {
                resource->m_Dependencies.insert(resource->m_Dependencies.end(), dependencies.begin(),
                                                dependencies.end());
            }
            resource->m_Path = resourceName;
            resource->m_Status = Resource::STATUS_DATA_FETCHING;

            m_Resources.push_back(resource);
        }

        handle.makeValid(this, reinterpret_cast<typename T::TYPE *>(resource));
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

    std::list<Resource *> m_Resources;
    ResourceFactory resourceFactory;
};
