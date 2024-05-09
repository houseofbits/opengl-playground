#pragma once

#include "../Components/Scene.h"
#include "../Helper/AtlasGraph.h"
#include "../Helper/ShaderSourceLoader.h"
#include "../PostProcessRenderer/PostProcessRenderer.h"
#include "BaseRenderer.h"
#include "Shader.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <list>

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