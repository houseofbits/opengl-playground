#include "ShadowMapResource.h"
#include <GL/glew.h>

ShadowMapResource::ShadowMapResource() : Resource(), m_textureRenderTarget(), m_handleId() {
}

Resource::Status ShadowMapResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status ShadowMapResource::build() {
    resize(512);

//    std::cout<<"Build "<<m_Path<<std::endl;
    return STATUS_READY;
}

void ShadowMapResource::destroy() {
    glMakeTextureHandleNonResidentARB(m_handleId);
    m_textureRenderTarget.destroy();
}

void ShadowMapResource::bindRenderTarget() {
    m_textureRenderTarget.bind();
}

void ShadowMapResource::unbindRenderTarget() {
    m_textureRenderTarget.unbind();
}

void ShadowMapResource::resize(int size) {
    destroy();

    m_textureRenderTarget.create(size, size, Texture::TYPE_RGBA32);

    m_handleId = glGetTextureHandleARB(m_textureRenderTarget.textureId);
    if (m_handleId == 0) {
        destroy();

        std::cout << "Error! Handle returned null on texture resize" << m_Path << std::endl;
    }

    glMakeTextureHandleResidentARB(m_handleId);
}
