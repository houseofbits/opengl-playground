#pragma once

#include "Renderer.h"
#include "Shader.h"

class ShadowMapRenderer
{
public:
    Shader shader;
    unsigned int depthFBOId;
    unsigned int depthTextureId;
    unsigned int mapSize;

    void init(unsigned int mapSize = 1024);
    void beginRender(Light *light);
    void endRender();
};