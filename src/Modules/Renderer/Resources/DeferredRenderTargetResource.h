#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Texture/TextureRenderTarget.h"
#include "../../Application/Resources/WindowResource.h"

class DeferredRenderTargetResource : public Resource {
public:
    DeferredRenderTargetResource();

    Resource::Status fetchData(ResourceManager &) override;

    Resource::Status build() override;

    void destroy() override;

    void bindRenderTarget();

    void unbindRenderTarget();

    [[nodiscard]] std::uint64_t getHandle() const {
        return m_textureRenderTarget.handleId;
    }

    TextureRenderTarget m_textureRenderTarget;
    ResourceHandle<WindowResource> m_WindowResourceHandle;
};
