#include "ShadowMapResource.h"
#include <GL/glew.h>

ShadowMapResource::ShadowMapResource() : Resource(), m_textureRenderTarget(),
                                         m_handleId(),
                                         m_Resolution(512) {
}

Resource::Status ShadowMapResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status ShadowMapResource::build() {
    if (!GLEW_ARB_bindless_texture) {
        return STATUS_BUILD_ERROR;
    }

    if (!GLEW_ARB_direct_state_access) {
        return STATUS_BUILD_ERROR;
    }

    resize();

    //    std::cout<<"Build "<<m_Path<<std::endl;
    return STATUS_READY;
}

void ShadowMapResource::destroy() {
    if (m_handleId > 0) {
        glMakeTextureHandleNonResidentARB(m_handleId);
    }
    m_textureRenderTarget.destroy();
}

void ShadowMapResource::bindRenderTarget() {
    m_textureRenderTarget.bind();
}

void ShadowMapResource::unbindRenderTarget() {
    m_textureRenderTarget.unbind();
}

void ShadowMapResource::resize() {
    destroy();

    if (!GLEW_ARB_bindless_texture) {
        return;
    }

    if (!GLEW_ARB_direct_state_access) {
        return;
    }

    m_textureRenderTarget.create(m_Resolution, m_Resolution, Texture::TYPE_RGBA32);

    m_handleId = glGetTextureHandleARB(m_textureRenderTarget.textureId);
    if (m_handleId == 0) {
        destroy();

        std::cout << "Error! Handle returned null on texture resize" << m_Path << std::endl;
    }

    glMakeTextureHandleResidentARB(m_handleId);
}
