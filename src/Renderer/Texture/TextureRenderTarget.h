#pragma once

#include "Texture.h"

class TextureRenderTarget : public Texture
{
private:
    unsigned int framebufferId;
    unsigned int renderbufferId;

public:
    TextureRenderTarget();

    void create(unsigned int width, unsigned int height, Texture::Type type);
    void bind();
    void unbind();
    void destroy();
};