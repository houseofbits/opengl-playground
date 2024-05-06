#pragma once

#include "TextureAtlasManager.h"
#include "LightsUniformBuffer.h"
#include "ShadowMapRenderer.h"
#include "StaticGeometryRenderer.h"

class RenderManager
{
    friend class StaticGeometryRenderer;

protected:
    Window* window;
    Camera* camera;
public:
    explicit RenderManager(Window*);

    LightsUniformBuffer lightsUniformBuffer;
    TextureAtlasManager atlasManager;
    ShadowMapRenderer shadowMapRenderer;
    StaticGeometryRenderer staticGeometryRenderer;

    void init();
    void render(Scene& scene);
    void setCamera(Camera* cam);
};