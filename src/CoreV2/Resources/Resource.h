#pragma once

#include <string>

class ResourceManager;

class Resource {
public:
    explicit Resource();
    virtual ~Resource() = default;

    inline static Resource *m_DefaultInstance = nullptr;

    enum Status {
        UNDEFINED,
        DATA_FETCHING,
        DATA_READY,
        READY,
        FETCH_ERROR,
        BUILD_ERROR,
    };

    std::string m_Path;
    Status m_Status = Status::UNDEFINED;
    unsigned int m_ReferenceCount = 0;

    virtual void fetchData(ResourceManager&) = 0;
    virtual void build() = 0;
    virtual void destroy() = 0;

    void setDataReadyStatus() {
        m_Status = DATA_READY;
    }
    void setFetchErrorStatus() {
        m_Status = FETCH_ERROR;
    }
    void setBuildErrorStatus() {
        m_Status = BUILD_ERROR;
    }
    void setReadyStatus() {
        m_Status = READY;
    }
    [[nodiscard]] bool isReady() const {
        return m_Status == READY;
    }
};
