#pragma once

#include <string>
#include <vector>

class ResourceManager;

class Resource {
public:
    explicit Resource();

    virtual ~Resource() = default;

    inline static Resource *m_DefaultInstance = nullptr;

    enum Status {
        STATUS_UNDEFINED,
        STATUS_DATA_FETCHING,
        STATUS_DATA_READY,
        STATUS_BUILDING,
        STATUS_READY,
        STATUS_FETCH_ERROR,
        STATUS_BUILD_ERROR,
    };

    std::string m_Path;
    Status m_Status = Status::STATUS_UNDEFINED;
    unsigned int m_ReferenceCount = 0;
    std::vector<std::string> m_Dependencies;

    virtual Status fetchData(ResourceManager &) {
        return STATUS_DATA_READY;
    }

    virtual Status build() {
        return STATUS_READY;
    }

    virtual void destroy() {
    }

    virtual void write() {
    }

    void setDataReady() {
        m_Status = STATUS_DATA_READY;
    }

    [[nodiscard]] bool isReady() const {
        return m_Status == STATUS_READY;
    }

    [[nodiscard]] bool isFinished() const {
        return m_Status == STATUS_UNDEFINED || m_Status == STATUS_READY || m_Status == STATUS_FETCH_ERROR || m_Status ==
               STATUS_BUILD_ERROR;
    }

    void addDependency(const std::string &path) {
        m_Dependencies.push_back(path);
    }
};
