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
    // std::cout << "size uint: " << sizeof(unsigned int) << std::endl;
    // std::cout << "size float: " << sizeof(float) << std::endl;
    // std::cout << "size vec4: " << sizeof(glm::vec4) << std::endl;
    // std::cout << "size vec3: " << sizeof(glm::vec3) << std::endl;
    // std::cout << "size vec2: " << sizeof(glm::vec2) << std::endl;
    // std::cout << "size mat4: " << sizeof(glm::mat4) << std::endl;
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
        lightsUniformData[index].isPointSource = light->type == Light::SPOT;
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

    glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_UNIFORM_BINDING_INDEX, lightsUniformBufferId);
    shader.setUniform("lights", lightsUniformBufferId);
    shader.setUniform("numActiveLights", numActiveLights);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapRenderer.shadowAtlas.targetTextureId);
    shader.setUniform("shadowDepthAtlas", 1);
    shader.setUniform("shadowAtlasRegions", shadowMapRenderer.shadowAtlasRegionBufferId);
}

void Renderer::updateLights(Scene &scene)
{
    shadowMapRenderer.atlasGraph.clearAll();

    unsigned int uniformIndex = 0;
    for (auto const &light : scene.lights)
    {
        light->generateViews(); // TODO Update only when necesarry

        for (unsigned int i = 0; i < light->numberOfViews; i++)
        {
            if (light->doesCastShadows)
            {
                populateUniformAtlasAttributes(lightsUniformData[uniformIndex], *light, light->views[i]);
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

    uniform.isPointSource = light.type == Light::SPOT;
    uniform.doesCastShadows = light.doesCastShadows && hasShadowAtlasIndex;
    uniform.position = light.position;
    uniform.color = light.color;
    uniform.distAttenMax = light.distAttenMax;
    uniform.distAttenMin = light.distAttenMin;
    uniform.intensity = light.intensity;
    uniform.direction = light.direction;
    uniform.beamAngle = glm::radians(light.beamAngle);
    uniform.projectionViewMatrix = view.projectionViewMatrix;
    uniform.shadowAtlasIndex = view.shadowAtlasIndex;
}

void Renderer::populateUniformAtlasAttributes(LightUniform &uniform, Light &light, LightView &view)
{
    unsigned int shadowMapSize = 256;

    int index = shadowMapRenderer.atlasGraph.occupyFirstAvailable(shadowMapSize, shadowMapSize);
    view.shadowAtlasIndex = index > 0 ? index : 0;
}

void Renderer::renderShadowAtlas(Scene &scene)
{
    shadowMapRenderer.beginRender();

    glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_UNIFORM_BINDING_INDEX, lightsUniformBufferId);
    shadowMapRenderer.depthShader.setUniform("lights", lightsUniformBufferId);
    shadowMapRenderer.depthShader.setUniform("numActiveLights", numActiveLights);

    // std::cout << numActiveLights << std::endl;

    int viewportIndex = 0;
    for (unsigned int i = 0; i < numActiveLights; i++)
    {
        if (lightsUniformData[i].doesCastShadows == 1)
        {
            AtlasNode &node = shadowMapRenderer.atlasGraph.nodes[lightsUniformData[i].shadowAtlasIndex];

            glViewportIndexedf(viewportIndex, node.left, node.top, node.width, node.height);

            // std::cout << viewportIndex << ", " << node.left << ", " << node.top << " " << node.width << ", " << node.height << std::endl;

            viewportIndex++;
        }
    }

    shadowMapRenderer.renderShadowAtlas(scene);
}

void Renderer::printDebugShadowMaps()
{
    shadowMapRenderer.atlasGraph.debug();

    for (unsigned int lightIndex = 0; lightIndex < numActiveLights; lightIndex++)
    {
        std::cout << lightIndex
                  << ": "
                  << lightsUniformData[lightIndex].shadowAtlasIndex << std::endl;

        glm::vec4 *p = shadowMapRenderer.atlasGraph.getRectangleArray(ShadowMapRenderer::ATLAS_WIDTH, ShadowMapRenderer::ATLAS_HEIGHT);
        std::cout << "atlas buff: "
                  << p[lightsUniformData[lightIndex].shadowAtlasIndex].x
                  << ","
                  << p[lightsUniformData[lightIndex].shadowAtlasIndex].y
                  << " "
                  << p[lightsUniformData[lightIndex].shadowAtlasIndex].z
                  << ","
                  << p[lightsUniformData[lightIndex].shadowAtlasIndex].w
                  << std::endl;
    }
}