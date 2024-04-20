#include "Renderer.h"
#include "Camera.h"
#include "Texture2D.h"
#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include "../Helper/ShaderSourceLoader.h"

Renderer::Renderer() : activeCamera(0)
{
    ShaderSourceLoader::registerGlobal("MAX_LIGHTS", MAX_LIGHTS);
    ShaderSourceLoader::registerGlobal("LIGHTS_UNIFORM_BINDING_INDEX", LIGHTS_UNIFORM_BINDING_INDEX);
    ShaderSourceLoader::registerGlobal("LIGHT_VIEWS_UNIFORM_BINDING_INDEX", LIGHT_VIEWS_UNIFORM_BINDING_INDEX);

    std::cout << "LightUniform size: " << sizeof(LightUniform) << std::endl;
}

void Renderer::init(Camera *camera)
{
    shadowMapRenderer.init();
    activeCamera = camera;
    initLightsBuffer();
}

void Renderer::initLightsBuffer()
{
    glGenBuffers(1, &lightsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUniform) * MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::generateLightsUniform(Scene &scene)
{
    unsigned int index = 0;
    for (auto const &light : scene.lights)
    {
        lightsUniformData[index].flags = light->getLightUniformFlags(true);
        lightsUniformData[index].position = light->position;
        lightsUniformData[index].color = light->color;
        lightsUniformData[index].distAttenMax = light->distAttenMax;
        lightsUniformData[index].distAttenMin = light->distAttenMin;
        lightsUniformData[index].intensity = light->intensity;
        lightsUniformData[index].direction = light->direction;
        lightsUniformData[index].beamAngle = glm::radians(light->beamAngle);

        light->uniformBufferIndex = index;

        index++;
    }

    numActiveLights = index;

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUniform) * numActiveLights, lightsUniformData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::setShaderAttributes(Shader &shader)
{
    shader.setUniform("viewPosition", activeCamera->getPosition());
    shader.setUniform("numActiveLights", numActiveLights);

    glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_UNIFORM_BINDING_INDEX, lightsUniformBufferId);
    shader.setUniform("lights", lightsUniformBufferId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapRenderer.shadowAtlas.targetTextureId);
    // glBindTexture(GL_TEXTURE_2D, shadowDepthMapId);
    shader.setUniform("shadowDepthAtlas", 1);

    shadowMapRenderer.setShaderAttributes(shader);
}

void Renderer::updateLights(Scene &scene)
{
    unsigned int shadowMapSize = 256;

    unsigned int uniformIndex = 0;
    for (auto const &light : scene.lights)
    {
        light->generateViews(); // TODO Update only when necesarry

        for (unsigned int i = 0; i < light->numberOfViews; i++)
        {
            if (light->doesCastShadows)
            {
                int index = shadowMapRenderer.atlasGraph.occupyFirstAvailable(shadowMapSize, shadowMapSize);
                light->views[i].shadowAtlasIndex = index > 0 ? index : 0;
                if (index > 0)
                {
                    AtlasNode &node = shadowMapRenderer.atlasGraph.nodes[index];
                    populateUniformAtlasAttributes(lightsUniformData[uniformIndex], node);
                }
            }

            populateUniform(lightsUniformData[uniformIndex], *light, light->views[i]);

            light->uniformBufferIndex = uniformIndex;

            uniformIndex++;
        }
    }

    numActiveLights = uniformIndex;

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUniform) * numActiveLights, lightsUniformData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::populateUniform(LightUniform &uniform, Light &light, LightView &view)
{
    bool hasShadowAtlasIndex = view.shadowAtlasIndex > 0;

    uniform.flags = light.getLightUniformFlags(hasShadowAtlasIndex);
    uniform.position = light.position;
    uniform.color = light.color;
    uniform.distAttenMax = light.distAttenMax;
    uniform.distAttenMin = light.distAttenMin;
    uniform.intensity = light.intensity;
    uniform.direction = light.direction;
    uniform.beamAngle = glm::radians(light.beamAngle);
    uniform.projectionViewMatrix = view.projectionViewMatrix;
}

void Renderer::populateUniformAtlasAttributes(LightUniform &uniform, AtlasNode &node)
{
    uniform.shadowAtlasPos = glm::vec2(
        (float)node.left / (float)ShadowMapRenderer::ATLAS_WIDTH,
        (float)node.top / (float)ShadowMapRenderer::ATLAS_HEIGHT);

    uniform.shadowAtlasSize = glm::vec2(
        (float)node.width / (float)ShadowMapRenderer::ATLAS_WIDTH,
        (float)node.height / (float)ShadowMapRenderer::ATLAS_HEIGHT);
}

void Renderer::renderShadowAtlas(Scene &scene)
{
    shadowMapRenderer.generateShadowAtlasViews(scene.lights);
    shadowMapRenderer.renderShadowAtlas(scene);
}