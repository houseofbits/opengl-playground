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

class ShadowMapRenderer
{
public:
    static const unsigned int MAX_LIGHTVIEWS_PER_PASS = 6;
    static const unsigned int ATLAS_WIDTH = 2048;
    static const unsigned int ATLAS_HEIGHT = 2048;

    ShadowMapRenderer();

    typedef struct alignas(16)
    {
        glm::mat4 lightProjectionViewMatrix;
        glm::vec2 atlasPos;
        glm::vec2 atlasSize;
        unsigned int lightIndex;
    } LightViewUniform;

    Shader depthShader;
    RenderTarget shadowAtlas;
    AtlasGraph atlasGraph;

    LightViewUniform lightViewUniformData[MAX_LIGHTVIEWS_PER_PASS];
    glm::uvec4 atlasViewports[MAX_LIGHTVIEWS_PER_PASS];
    unsigned int lightViewsUniformBufferId;
    unsigned int numberOfLightViews;

    void init();
    void generateShadowAtlasViews(std::list<Light *> &lights);
    void renderShadowAtlas(Scene &scene);
    void setShaderAttributes(Shader &shader);
};