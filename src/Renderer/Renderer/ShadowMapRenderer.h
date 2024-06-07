#pragma once

#include "../../Components/Scene.h"
#include "../../Helper/AtlasGraph.h"
#include "../../Helper/ShaderSourceLoader.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderStorageBuffer.h"
#include "BaseRenderer.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <list>

class ShadowMapRenderer : public BaseRenderer
{
private:
    typedef ShaderStorageBuffer<unsigned int> IndexBuffer;

    static const unsigned int MAX_LIGHTVIEWS_PER_PASS = 16;     //Hard limit by GL_MAX_VIEWPORTS

    void prepareViewports();
    void renderScene(Scene &scene);

    IndexBuffer lightIndexesUniform;
    unsigned int numLightsPerPass;

public:
    explicit ShadowMapRenderer(RenderManager *);

    Shader depthShader;
    PostProcessRenderer debugImageRenderer;

    void init(unsigned int uniformBindingIndex);
    void render(Scene &scene);
    void renderShadowAtlas();
};