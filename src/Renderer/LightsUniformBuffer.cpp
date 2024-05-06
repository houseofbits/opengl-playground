#include "LightsUniformBuffer.h"
#include "../Helper/ShaderSourceLoader.h"

LightsUniformBuffer::LightsUniformBuffer() : uniformBuffer(), maxActiveLights(0), numActiveLights(0)
{
    ShaderSourceLoader::registerGlobal("LIGHTS_UNIFORM_BINDING_INDEX", LIGHTS_UNIFORM_BINDING_INDEX);
}

void LightsUniformBuffer::create(unsigned int maxLights)
{
    maxActiveLights = maxLights;
    uniformBuffer.create(maxActiveLights, LightUniformBuffer::DYNAMIC, 1);

    ShaderSourceLoader::registerGlobal("MAX_LIGHTS", maxActiveLights);
}

void LightsUniformBuffer::update(Scene &scene, TextureAtlasManager &atlasManager)
{
    atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).qtOccupancy.clear();

    unsigned int uniformIndex = 0;
    for (auto const &light : scene.lights)
    {
        light->generateViews(); // TODO Update only when necessary

        LightUniform &uniform = uniformBuffer.get(uniformIndex);

        for (unsigned int i = 0; i < light->numberOfViews; i++)
        {
            if (light->doesCastShadows)
            {
                light->views[i].shadowAtlasIndex = getShadowAtlasRegionIndex(atlasManager);
            }

            populateUniform(uniform, *light, light->views[i]);

            light->uniformBufferIndex = uniformIndex;

            uniformIndex++;
        }
    }

    numActiveLights = uniformIndex;
    uniformBuffer.update(0, numActiveLights);
}

void LightsUniformBuffer::populateUniform(LightUniform &uniform, Light &light, LightView &view)
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

unsigned int LightsUniformBuffer::getShadowAtlasRegionIndex(TextureAtlasManager &atlasManager)
{
    int shadowMapSize = 512;

    int index = atlasManager.occupyRegion(TextureAtlasManager::ATLAS_SHADOW_DEPTH, shadowMapSize);
    return index > 0 ? index : 0;
}

void LightsUniformBuffer::bind(Shader &shader)
{
    uniformBuffer.bind();
    shader.setUniform("lights", uniformBuffer.getBufferId());
    shader.setUniform("numActiveLights", numActiveLights);
}