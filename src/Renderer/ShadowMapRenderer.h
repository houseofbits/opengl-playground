#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
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

    ShadowMapRenderer();

    Shader depthShader;
    PostProcessRenderer debugImageRenderer;

    void init();
    void renderShadowAtlas(Scene &scene);
    void debugRender();

    void render(Scene &scene, TextureAtlasManager &atlasManager);
};