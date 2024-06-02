#pragma once

#include "EnvironmentProbe/EnvironmentProbeUniformBuffer.h"
#include "Light/LightsUniformBuffer.h"
#include "Renderer/DebugInformationRenderer.h"
#include "Renderer/EnvironmentProbeRenderer.h"
#include "Renderer/ShadowMapRenderer.h"
#include "Renderer/SkyBoxRenderer.h"
#include "Renderer/StaticGeometryRenderer.h"
#include "TextureAtlas/TextureAtlasManager.h"

class Window;

class RenderManager
{
    friend class StaticGeometryRenderer;

protected:
    Window* window;

    PostProcessRenderer debugRendererColor;
public:
    static const unsigned int LIGHTS_UNIFORM_BINDING_INDEX = 1;
    static const unsigned int ATLAS_REGION_MAPPING_UNIFORM_BINDING_INDEX = 2;
    static const unsigned int LIGHT_INDEXES_UNIFORM_BINDING_INDEX = 3;
    static const unsigned int ENV_PROBES_UNIFORM_BINDING_INDEX = 4;


    explicit RenderManager(Window*);

    LightsUniformBuffer lightsUniformBuffer;
    EnvironmentProbeUniformBuffer environmentProbeUniformBuffer;
    TextureAtlasManager atlasManager;
    ShadowMapRenderer shadowMapRenderer;
    StaticGeometryRenderer staticGeometryRenderer;
    SkyBoxRenderer skyBoxRenderer;
    DebugInformationRenderer debugInformationRenderer;
    EnvironmentProbeRenderer environmentProbeRenderer;

    void init();
    void preRender(Scene& scene);
    void render(Scene& scene);
    void renderDebug(int mode);

    bool isLightConesVisible;
};