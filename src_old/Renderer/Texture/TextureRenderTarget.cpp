#include "TextureRenderTarget.h"
#include <iostream>
#include <GL/glew.h>

TextureRenderTarget::TextureRenderTarget() : Texture(), framebufferId(0), renderbufferId(0)
{

}

void TextureRenderTarget::create(unsigned int width, unsigned int height, Texture::Type type)
{
    glGenFramebuffers(1, &framebufferId);

    Texture::create(width, height, type);

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    if (type == Texture::TYPE_DEPTH)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        glGenRenderbuffers(1, &renderbufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureRenderTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    Texture::bind();
}

void TextureRenderTarget::destroy()
{
    Texture::destroy();
    glDeleteFramebuffers(1, &framebufferId);
    glDeleteRenderbuffers(1, &renderbufferId);
}

void TextureRenderTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

