#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

class Renderer
{
public:
    Renderer();

    static const unsigned int MAX_LIGHTS = 10;

    Camera *activeCamera;
    std::list<Light *> lights;
    Light::ShaderLight shaderLights[Renderer::MAX_LIGHTS];
    unsigned int lightsUniformBufferId;
    unsigned int numActiveLights;

    unsigned int testTextureId;
    unsigned int shadowDepthMapId;

    Light *createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity = 1.);
    Light *createDirectLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity = 1.);
    void init(Camera *camera);
    void initLightsBuffer();
    void updateLights();
    void setShaderGlobalAttributes(Shader *shader);
};