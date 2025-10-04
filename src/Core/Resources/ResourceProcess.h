#pragma once
#include <atomic>
#include <thread>

class Resource;
class ResourceManager;

class ResourceProcess {
public:
    explicit ResourceProcess(ResourceManager *);

    ~ResourceProcess();

    void buildFetchedResources() const;

private:
    void fetchProcess() const;

    bool areDependenciesReady(Resource *resource) const;

    bool doesDependencyHaveError(Resource *resource) const;

    std::thread m_FetchThread;
    std::atomic<bool> m_FetchProcessRunning;
    ResourceManager *m_resourceManager;
};
