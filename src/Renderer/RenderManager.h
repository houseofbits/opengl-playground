#pragma once

#include "TextureAtlasManager.h"
#include "LightsUniformBuffer.h"
#include "ShadowMapRenderer.h"

class RenderManager
{
public:
    LightsUniformBuffer lightsUniformBuffer;
    TextureAtlasManager atlasManager;

    ShadowMapRenderer shadowMapRenderer;
};