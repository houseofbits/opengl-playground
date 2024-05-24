#pragma once

#include "Light/LightsUniformBuffer.h"
#include "Renderer/ShadowMapRenderer.h"
#include "Renderer/SkyBoxRenderer.h"
#include "Renderer/StaticGeometryRenderer.h"
#include "Texture/TextureAtlasManager.h"

class Window;

class RenderManager
{
    friend class StaticGeometryRenderer;

protected:
    Window* window;

    PostProcessRenderer debugRendererColor;
public:
    explicit RenderManager(Window*);

    LightsUniformBuffer lightsUniformBuffer;
    TextureAtlasManager atlasManager;
    ShadowMapRenderer shadowMapRenderer;
    StaticGeometryRenderer staticGeometryRenderer;
    SkyBoxRenderer skyBoxRenderer;

    TextureCubeRenderTarget cubeRenderTarget;
    Camera cubeCamera;

    void init();
    void render(Scene& scene);
    void renderDebug(int mode);
};