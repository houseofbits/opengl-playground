#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Texture/TextureRenderTarget.h"
#include "ShaderProgramResource.h"

class ShadowMapResource : public Resource {
public:
    ShadowMapResource();

    Resource::Status fetchData(ResourceManager &) override;
    Resource::Status build() override;
    void destroy() override;
    void bindRenderTarget();
    void unbindRenderTarget();
    void resize();

    TextureRenderTarget m_textureRenderTarget;
    std::uint64_t m_handleId;
    int m_Resolution;
};
