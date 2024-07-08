#include "EnvironmentProbesCubeMapArrayResource.h"
#include <GL/glew.h>

EnvironmentProbesCubeMapArrayResource::EnvironmentProbesCubeMapArrayResource() : Resource(),
                                                                                 m_TextureCube(),
                                                                                 m_framebufferId(),
                                                                                 m_renderbufferId() {
}

Resource::Status EnvironmentProbesCubeMapArrayResource::build() {
    glGenFramebuffers(1, &m_framebufferId);

    m_TextureCube.createArray(TEXTURE_SIZE, TEXTURE_SIZE, Texture::TYPE_RGBA, TEXTURE_DEPTH);

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
    glGenRenderbuffers(1, &m_renderbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, TEXTURE_SIZE, TEXTURE_SIZE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return Resource::STATUS_READY;
}

void EnvironmentProbesCubeMapArrayResource::destroy() {
    m_TextureCube.destroy();
    glDeleteFramebuffers(1, &m_framebufferId);
    glDeleteRenderbuffers(1, &m_renderbufferId);
}

void EnvironmentProbesCubeMapArrayResource::bindRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
    m_TextureCube.bind();
}

void EnvironmentProbesCubeMapArrayResource::unbindRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EnvironmentProbesCubeMapArrayResource::generateMipMaps() {
    glGenerateTextureMipmap(m_TextureCube.textureId);
}

void EnvironmentProbesCubeMapArrayResource::selectCubeFace(int face) {
    glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureCube.textureId, 0, face);
}
