#include "ResourceProcess.h"
#include "ResourceManager.h"

ResourceProcess::ResourceProcess(ResourceManager *manager) : m_resourceManager(manager),
                                                             m_FetchThread(&ResourceProcess::fetchProcess, this),
                                                             m_FetchProcessRunning(true) {
}

ResourceProcess::~ResourceProcess() {
    m_FetchProcessRunning = false;
    m_FetchThread.join();
}

void ResourceProcess::fetchProcess() const {
    //        const auto timestep = std::chrono::milliseconds(100);
    while (true) {
        if (!m_FetchProcessRunning) {
            return;
        }
        auto start = std::chrono::steady_clock::now();
        for (const auto &resource: m_resourceManager->m_Resources) {
            if (resource->m_Status == Resource::STATUS_DATA_FETCHING) {
                if (areDependenciesReady(resource)) {
                    resource->m_Status = resource->fetchData(*m_resourceManager);
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

bool ResourceProcess::areDependenciesReady(Resource *resource) const {
    // Log::write("Check resource ", resource->m_Path);
    if (!resource->m_Dependencies.empty()) {
        for (const auto &dependencyPath: resource->m_Dependencies) {
            const Resource *r = m_resourceManager->findResource(dependencyPath);
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

bool ResourceProcess::doesDependencyHaveError(Resource *resource) const {
    if (!resource->m_Dependencies.empty()) {
        for (const auto &dependencyPath: resource->m_Dependencies) {
            Resource *r = m_resourceManager->findResource(dependencyPath);
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

void ResourceProcess::buildFetchedResources() const {
    for (const auto &resource: m_resourceManager->m_Resources) {
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
