#pragma once

#include "TextureCube.h"

class TextureCubeRenderTarget : public TextureCube
{
private:
    unsigned int framebufferId;
    unsigned int renderbufferId;

public:
    TextureCubeRenderTarget();

    void create(unsigned int width, unsigned int height, Texture::Type type);
    void bind();
    void unbind();
    void destroy();
};
