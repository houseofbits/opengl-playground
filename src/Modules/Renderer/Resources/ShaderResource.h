#pragma once

#include "../../../Core/API.h"

class ShaderResource : public Resource {
public:
    ShaderResource();

    Status fetchData(ResourceManager&) override;
    Status build() override;
    void destroy() override;

    static unsigned int getShaderType(const std::string &filename);
    [[nodiscard]] bool checkCompileError() const;

    std::string m_SourceCode;
    unsigned int m_Handle;

private:
    static bool areComputeShadersSupported();
};
