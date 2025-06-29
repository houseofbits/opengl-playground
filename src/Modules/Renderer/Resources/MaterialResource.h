#pragma once

#include "../../../Core/API.h"
#include "ShaderProgramResource.h"
#include "TextureResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include "../../../Renderer/Material/MaterialConfiguration.h"

class MaterialResource : public Resource {
public:
    MaterialResource();

    Status build() override;

    void destroy() override;

    void fetchFromGLTF(ResourceManager &resourceManager, const tinygltf::Model &model,
                       const tinygltf::Material &material,
                       const std::string& basePath = "");

    void fetchDefault(ResourceManager &resourceManager);

    void bind(ShaderProgramResource &shader);

    MaterialConfiguration m_materialConfiguration;
    ResourceHandle<TextureResource> m_Diffuse;
    ResourceHandle<TextureResource> m_Normal;
    ResourceHandle<TextureResource> m_Roughness;
    ResourceHandle<TextureResource> m_Emissive;

private:
    void requestTextureResource(ResourceManager &, ResourceHandle<TextureResource> &, const std::string &);
};
