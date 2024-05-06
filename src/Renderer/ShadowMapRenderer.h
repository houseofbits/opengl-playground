#pragma once

#include "Scene.h"
#include "Shader.h"
#include "../Helper/AtlasGraph.h"
#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include "../Helper/ShaderSourceLoader.h"
#include "../PostProcessRenderer/PostProcessRenderer.h"
#include "BaseRenderer.h"

class ShadowMapRenderer : public BaseRenderer
{
private:
    static const unsigned int MAX_LIGHTVIEWS_PER_PASS = 20;

    void prepareViewports();

public:
    explicit ShadowMapRenderer(RenderManager *);

    Shader depthShader;
    PostProcessRenderer debugImageRenderer;

    void init();
    void render(Scene &scene);
    void renderShadowAtlas();
};