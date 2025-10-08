#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/Shader.h"

class ShaderUniformResource;
class Camera;

class ShaderResource : public Resource {
public:
    ShaderResource();

    typedef ResourceHandle<ShaderUniformResource> ShaderUniformResourceHandle;

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override;

    void use(Camera &camera);

    Shader& getShader() {
        return m_shader;
    }

private:
    void loadShader();

    void fetchAttributes(nlohmann::json &);

    void fetchDependencies(nlohmann::json &, ResourceManager &);

    bool m_fetchMetadata = true;
    std::string m_shaderPath;
    Shader m_shader;
    std::vector<ShaderUniformResourceHandle *> m_uniformResources;
    std::vector<std::pair<std::string, ShaderUniformResourceHandle *> > m_namedUniformResources;
    bool m_isDepthTestEnabled;
};
