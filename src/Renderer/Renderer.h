#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include "Shader.h"
#include "ShadowMapRenderer.h"
#include "RenderTarget.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "../Helper/AtlasGraph.h"

class Renderer
{
public:
    Renderer();

    static const unsigned int LIGHTS_UNIFORM_BINDING_INDEX = 1;
    static const unsigned int LIGHT_VIEWS_UNIFORM_BINDING_INDEX = 2;

    static const unsigned int MAX_LIGHTS = 10;

    Camera *activeCamera;
    LightUniform lightsUniformData[Renderer::MAX_LIGHTS];
    unsigned int lightsUniformBufferId;
    unsigned int numActiveLights;

    unsigned int shadowDepthMapId;

    void init(Camera *camera);
    void initLightsBuffer();
    void generateLightsUniform(Scene &scene);
    void setShaderAttributes(Shader &shader);

    // New stuff
    ShadowMapRenderer shadowMapRenderer;
    void updateLights(Scene &scene);
    void populateUniform(LightUniform &uniform, Light &light, LightView &view);
    void populateUniformAtlasAttributes(LightUniform &uniform, Light &light, LightView &view);
    void renderShadowAtlas(Scene &scene);

    void printDebugShadowMaps();
};