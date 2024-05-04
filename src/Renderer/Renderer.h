#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include "Shader.h"
#include "ShadowMapRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "UniformBuffer.h"
#include "TextureAtlasManager.h"
#include "LightsUniformBuffer.h"
#include "../Helper/AtlasGraph.h"

class Renderer
{
public:
    typedef UniformBuffer<LightUniform> LightUniformBuffer;

public:
    Renderer();

    static const unsigned int LIGHTS_UNIFORM_BINDING_INDEX = 1;
    static const unsigned int LIGHT_VIEWS_UNIFORM_BINDING_INDEX = 2;

    void init(Camera *camera);
    void setShaderAttributes(Shader &shader);
    void updateLights(Scene &scene);
    void renderShadowAtlas(Scene &scene);
    void printDebugShadowMaps();

    TextureAtlasManager atlasManager;
    Camera *activeCamera;
    LightsUniformBuffer lightsUniformBuffer;
    ShadowMapRenderer shadowMapRenderer;
};