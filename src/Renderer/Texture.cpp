#include "Texture.h"
#include <GL/glew.h>

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::create(unsigned int textureWidth, unsigned int textureHeight, Type textureType)
{
    type = textureType;
    width = textureWidth;
    height = textureHeight;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (type == TYPE_DEPTH)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureStorage2D(textureId, 1, GL_RGB8, width, height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Texture::applyImage(unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData)
{
    glTextureSubImage2D(textureId, 0, left, top, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);
}

void Texture::destroy()
{
    glDeleteTextures(1, &textureId);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}