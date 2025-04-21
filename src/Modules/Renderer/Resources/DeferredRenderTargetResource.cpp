#include "DeferredRenderTargetResource.h"
#include "ShadowMapResource.h"
#include <GL/glew.h>

DeferredRenderTargetResource::DeferredRenderTargetResource() : Resource(), m_textureRenderTarget() {
}

Resource::Status DeferredRenderTargetResource::fetchData(ResourceManager &resourceManager) {
    addDependency("window");
    resourceManager.request(m_WindowResourceHandle, "window");

    return Resource::STATUS_DATA_READY;
}

Resource::Status DeferredRenderTargetResource::build() {
    if (!GLEW_ARB_bindless_texture) {
        return STATUS_BUILD_ERROR;
    }

    if (!GLEW_ARB_direct_state_access) {
        return STATUS_BUILD_ERROR;
    }

    const auto viewportWidth = m_WindowResourceHandle().m_Window->viewportWidth;
    const auto viewportHeight = m_WindowResourceHandle().m_Window->viewportHeight;

    m_textureRenderTarget.create(viewportWidth, viewportHeight, Texture::TYPE_RGBA);

    return Resource::STATUS_READY;
}

void DeferredRenderTargetResource::destroy() {
    m_textureRenderTarget.destroy();
}

void DeferredRenderTargetResource::bindRenderTarget() {
    m_textureRenderTarget.bind();
}

void DeferredRenderTargetResource::unbindRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
