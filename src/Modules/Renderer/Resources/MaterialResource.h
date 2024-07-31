#pragma once

#include "../../../Core/API.h"
#include "ShaderProgramResource.h"
#include "TextureResource.h"

class MaterialResource : public Resource {
public:
    inline static const std::string DIFFUSE_TEXTURE_KEY = "diffuseTexture";
    inline static const std::string DIFFUSE_COLOR_KEY = "diffuseColor";
    inline static const std::string SELF_ILLUMINATION_KEY = "selfIllumination";
    inline static const std::string NORMAL_TEXTURE_KEY = "normalTexture";
    inline static const std::string ROUGHNESS_TEXTURE_KEY = "roughnessTexture";
    inline static const std::string CAST_SHADOWS_KEY = "doesCastShadows";
    inline static const std::string RECEIVE_SHADOWS_KEY = "doesReceiveShadows";

    MaterialResource();

    Resource::Status fetchData(ResourceManager &) override;
    Resource::Status build() override;
    void destroy() override;
    void write() override;

    void bind(ShaderProgramResource &shader);

    glm::vec3 m_DiffuseColor;
    float m_selfIllumination;
    ResourceHandle<TextureResource> m_Diffuse;
    ResourceHandle<TextureResource> m_Normal;
    ResourceHandle<TextureResource> m_Roughness;
    bool m_doesCastShadows;
    bool m_doesReceiveShadows;
};
