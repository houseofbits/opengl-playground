#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Texture/TextureRenderTarget.h"

class RenderTargetResource : public Resource {
public:
    RenderTargetResource();

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override;

    void bindRenderTarget();

    static void unbindRenderTarget();

    void resize(unsigned int width, unsigned int height, Texture::Type type);

    [[nodiscard]] std::uint64_t getHandle() const {
        return m_textureRenderTarget.handleId;
    }

    TextureRenderTarget m_textureRenderTarget;
    unsigned int m_width;
    unsigned int m_height;
    Texture::Type m_type;
};
