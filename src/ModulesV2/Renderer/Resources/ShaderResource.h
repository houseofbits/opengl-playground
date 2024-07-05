#pragma once

#include "../../../CoreV2/API.h"

class ShaderResource : public Resource {
public:
    ShaderResource();

    Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    static unsigned int getShaderType(const std::string &filename);
    bool checkCompileError();

    std::string m_SourceCode;
    unsigned int m_Handle;
};
