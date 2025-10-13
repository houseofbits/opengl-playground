#include "RenderTargetResource.h"
#include <GL/glew.h>

RenderTargetResource::RenderTargetResource() : Resource(), m_textureRenderTarget(), m_width(256), m_height(256),
                                               m_type(Texture::TYPE_RGBA) {
}

Resource::Status RenderTargetResource::fetchData(ResourceManager &resourceManager) {
    addDependency("window");

    return Resource::STATUS_DATA_READY;
}

Resource::Status RenderTargetResource::build() {
    if (!GLEW_ARB_bindless_texture) {
        return STATUS_BUILD_ERROR;
    }

    if (!GLEW_ARB_direct_state_access) {
        return STATUS_BUILD_ERROR;
    }

    return Resource::STATUS_READY;
}

void RenderTargetResource::destroy() {
    m_textureRenderTarget.destroy();
}

void RenderTargetResource::bindRenderTarget() {
    m_textureRenderTarget.bind();
}

void RenderTargetResource::unbindRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTargetResource::resize(unsigned int width, unsigned int height, Texture::Type type) {
    destroy();

    m_width = width;
    m_height = height;
    m_type = type;

    m_textureRenderTarget.create(m_width, m_height, m_type);
    m_textureRenderTarget.createHandle();
}
