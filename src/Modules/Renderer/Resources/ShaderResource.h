#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/Shader.h"

class ShaderUniformResource;

class ShaderResource : public Resource {
public:
    ShaderResource();

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override;

    Shader m_shader;
    std::vector<ResourceHandle<ShaderUniformResource> *> m_uniformResources;
    bool m_isDepthTestEnabled;
};
