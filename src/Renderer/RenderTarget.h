#pragma once
#include <GL/glew.h>
#include <iostream>

class RenderTarget
{
public:
    enum Target
    {
        TARGET_DEPTH,
        TARGET_COLOR,
    };

    Target target;
    unsigned int width;
    unsigned int height;
    unsigned int framebufferId;
    unsigned int targetTextureId;
    unsigned int renderbufferId;
    bool isClearBuffersEnabled;

    void create(Target target, unsigned int imageWidth, unsigned int imageHeight);
    void beginRenderPartial(unsigned int fromX, unsigned int fromY, unsigned int sizeX, unsigned int sizeY);
    void beginRender();
    void end();
    void blit(unsigned int screenWidth, unsigned int screenHeight);
};