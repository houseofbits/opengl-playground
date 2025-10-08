#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/RenderShader.h"

class RenderShaderResource;
class Camera;
class ShaderBuilder;

class ShaderResource : public Resource {
public:
    ShaderResource();

    typedef ResourceHandle<RenderShaderResource> RenderShaderResourceHandle;

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override;

    void use(Camera &camera);

    RenderShader& getShader() {
        return m_shader;
    }

private:
    void loadShader();

    void fetchAttributes(nlohmann::json &);

    void fetchDependencies(nlohmann::json &, ResourceManager &);

    bool m_fetchMetadata = true;
    std::string m_shaderPath;
    RenderShader m_shader;
    std::vector<RenderShaderResourceHandle *> m_uniformResources;
    std::vector<std::pair<std::string, RenderShaderResourceHandle *> > m_namedUniformResources;
    bool m_isDepthTestEnabled;
    ShaderBuilder* m_shaderBuilder;
};
