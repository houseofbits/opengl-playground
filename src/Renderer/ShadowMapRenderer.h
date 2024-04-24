#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "RenderTarget.h"
#include "../Helper/AtlasGraph.h"
#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include "../Helper/ShaderSourceLoader.h"
#include "../PostProcessRenderer/PostProcessRenderer.h"

class ShadowMapRenderer
{
public:
    static const unsigned int MAX_LIGHTVIEWS_PER_PASS = 20;
    static const unsigned int ATLAS_WIDTH = 2048;
    static const unsigned int ATLAS_HEIGHT = 2048;

    ShadowMapRenderer();

    Shader depthShader;
    RenderTarget shadowAtlas;
    AtlasGraph atlasGraph;
    PostProcessRenderer debugImageRenderer;

    unsigned int shadowAtlasRegionBufferId;

    void init();
    void beginRender();
    void renderShadowAtlas(Scene &scene);
    void debugRender();
    void generateAtlasRegionUniformBuffer();
};