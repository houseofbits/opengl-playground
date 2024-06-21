#pragma once

#include "../CoreV2/Resources/Resource.h"

class ShaderResource : public Resource {
public:
    ShaderResource();

    void fetchData(ResourceManager&) override;
    void build() override;
    void destroy() override;

    static unsigned int getShaderType(const std::string &filename);
    bool checkCompileError();

    std::string m_SourceCode;
    unsigned int m_Handle;
};
