#pragma once

#include "Texture.h"

class TextureCube : public Texture
{
public:
    TextureCube();

    void create(unsigned int width, unsigned int height, Type type) override;
    void createArray(unsigned int width, unsigned int height, Type type, int depth) override;
    void applyImageToFace(unsigned int face, unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData);
    void bind() override;
    void unbind() override;

private:
    void fillWithTemporaryData();
};