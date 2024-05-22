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
    typedef UniformBuffer<unsigned int> UniformIndexBuffer;

    static const unsigned int MAX_LIGHTVIEWS_PER_PASS = 16;     //Hard limit by GL_MAX_VIEWPORTS
    static const unsigned int LIGHT_INDEXES_UNIFORM_BINDING_INDEX = 3;

    void prepareViewports();
    void prepareIndexes(unsigned int page);
    unsigned int calculateNumPages();

    UniformIndexBuffer lightIndexesUniform;
    unsigned int numLightsPerPass;

public:
    explicit ShadowMapRenderer(RenderManager *);

    Shader depthShader;
    PostProcessRenderer debugImageRenderer;

    void init();
    void render(Scene &scene);
    void renderShadowAtlas();
};