#include "Renderer.h"
#include "Camera.h"
#include "Texture2D.h"
#include <GL/glew.h>
#include <iostream>
#include <math.h>

Renderer::Renderer() : activeCamera(0)
{
    std::cout << "Size: " << sizeof(Light::ShaderLight) << std::endl;
}

void Renderer::init(Camera *camera)
{
    activeCamera = camera;
    initLightsBuffer();

    testTextureId = Texture2D::createTexture("resources/textures/checker-map.png");
}

void Renderer::initLightsBuffer()
{
    glGenBuffers(1, &lightsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Light::ShaderLight) * MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateLights()
{
    unsigned int index = 0;
    for (auto const &light : lights)
    {
        shaderLights[index].type = light->type;
        shaderLights[index].position = light->position;
        shaderLights[index].color = light->color;
        shaderLights[index].distAttenMax = light->distAttenMax;
        shaderLights[index].distAttenMin = light->distAttenMin;
        shaderLights[index].intensity = light->intensity;

        if (light->type == Light::SPOT)
        {
            shaderLights[index].direction = light->direction;
            shaderLights[index].beamAngle = light->beamAngle;
        }

        if (light->doesCastShadows && light->type == Light::SPOT)
        {
            shaderLights[index].type = Light::SPOT_SHADOW;
        }

        index++;
    }

    numActiveLights = lights.size();

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light::ShaderLight) * numActiveLights, shaderLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Light *Renderer::createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity)
{
    Light *light = new Light();

    light->type = Light::POINT;
    light->position = pos;
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->doesCastShadows = false;

    lights.push_back(light);

    return light;
}

Light *Renderer::createDirectLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity)
{
    Light *light = new Light();

    light->type = Light::SPOT;
    light->position = pos;
    light->direction = direction;
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->beamAngle = (beamAngle * (M_PI / 180)) / 2.0;
    light->doesCastShadows = false;

    lights.push_back(light);

    return light;
}

void Renderer::setShaderGlobalAttributes(Shader *shader)
{
    shader->setUniform("viewPosition", activeCamera->getPosition());
    shader->setUniform("numActiveLights", numActiveLights);
    shader->setUniform("lights", lightsUniformBufferId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowDepthMapId);
    shader->setUniform("lightDepthMap1", 1);

    Light *shadowLight = getFirstLightWithShadow();
    if (shadowLight != nullptr)
    {
        shader->setUniform("lightViewMatrix", shadowLight->camera.getProjectionViewMatrix());
    }
}

Light *Renderer::getFirstLightWithShadow()
{
    for (const auto &light : lights)
    {
        if (light->doesCastShadows)
        {
            return light;
        }
    }

    return nullptr;
}