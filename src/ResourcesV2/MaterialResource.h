#pragma once

#include "../CoreV2/Resources/Resource.h"
#include "../CoreV2/Resources/ResourceHandle.h"
#include "ShaderProgramResource.h"
#include "TextureResource.h"

class MaterialResource : public Resource {
public:
    inline static const std::string DIFFUSE_TEXTURE_KEY = "diffuseTexture";
    inline static const std::string NORMAL_TEXTURE_KEY = "normalTexture";

    MaterialResource();

    void fetchData(ResourceManager&) override;
    void build() override;
    void destroy() override;

    void bind(ShaderProgramResource &shader);

    ResourceHandle<TextureResource> m_Diffuse;
    ResourceHandle<TextureResource> m_Normal;
};
