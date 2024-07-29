#include "TextureCubeRenderTarget.h"
#include <GL/glew.h>
#include <iostream>

TextureCubeRenderTarget::TextureCubeRenderTarget() : TextureCube(),framebufferId(0), renderbufferId(0)
{

}

void TextureCubeRenderTarget::create(unsigned int width, unsigned int height, Texture::Type type)
{
    glGenFramebuffers(1, &framebufferId);

    TextureCube::create(width, height, type);

    initBuffers();
}

void TextureCubeRenderTarget::createArray(unsigned int width, unsigned int height, Texture::Type type, int depth)
{
    glGenFramebuffers(1, &framebufferId);

    TextureCube::createArray(width, height, type, depth);

    initBuffers();
}

void TextureCubeRenderTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    TextureCube::bind();
}

void TextureCubeRenderTarget::destroy()
{
    Texture::destroy();
}

void TextureCubeRenderTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void TextureCubeRenderTarget::initBuffers() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    if (type == Texture::TYPE_DEPTH)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else
    {
        glGenRenderbuffers(1, &renderbufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)width, (int)height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
