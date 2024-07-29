#pragma once

#include "TextureCube.h"

class TextureCubeRenderTarget : public TextureCube {
private:
    unsigned int framebufferId;
    unsigned int renderbufferId;

public:
    TextureCubeRenderTarget();

    void create(unsigned int width, unsigned int height, Texture::Type type) override;
    void createArray(unsigned int w, unsigned int h, Texture::Type t, int depth) override;
    void bind() override;
    void unbind() override;
    void destroy() override;

private:
    void initBuffers();
};
