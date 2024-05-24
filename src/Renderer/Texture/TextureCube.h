#pragma once

#include "Texture.h"

class TextureCube : public Texture
{
public:
    TextureCube();

    virtual void create(unsigned int width, unsigned int height, Type type);
    void applyImageToFace(unsigned int face, unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData);

    virtual void bind();
    virtual void unbind();
};