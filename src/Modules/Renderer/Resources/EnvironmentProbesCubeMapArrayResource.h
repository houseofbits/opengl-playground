#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Texture/TextureCube.h"
#include "ShaderUniformResource.h"

class EnvironmentProbesCubeMapArrayResource : public ShaderUniformResource {
public:
    inline static const int TEXTURE_SIZE = 256;
    inline static const int TEXTURE_DEPTH = 256;

    EnvironmentProbesCubeMapArrayResource();

    Resource::Status build() override;

    void destroy() override;

    void bindRenderTarget();

    void unbindRenderTarget();

    void generateMipMaps();

    void selectCubeFace(int layer);

    void use(Shader &shader) override;

    TextureCube m_TextureCube;
    std::uint64_t m_handleId;
    unsigned int m_framebufferId;
    unsigned int m_renderbufferId;
};
