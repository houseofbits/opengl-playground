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
//    std::cout<<"===================="<<std::endl;
    for (auto const &light : scene.lights)
    {
        light->generateViews(); // TODO Update only when necessary

        int textureSize = light->calculateMaximumTextureSize();

        for (unsigned int i = 0; i < light->numberOfViews; i++)
        {
            LightUniform &uniform = uniformBuffer.get(uniformIndex);

            if (light->doesCastShadows)
            {
                light->views[i].shadowAtlasIndex = getShadowAtlasRegionIndex(atlasManager, textureSize);
            }

//            std::cout<<uniformIndex<<" - "<<light->views[i].shadowAtlasIndex<<std::endl;

            populateUniform(uniform, *light, light->views[i]);

            light->uniformBufferIndex = uniformIndex;

//            std::cout<<uniformIndex<<std::endl;

            uniformIndex++;
        }
    }

//        Light* light = (*scene.lights.begin());
//        light->generateViews();
//        for(unsigned int i=0; i<30; i++)
//        {
//            LightUniform &uniform = uniformBuffer.get(uniformIndex);
//
//            if (light->doesCastShadows) {
//                light->views[0].shadowAtlasIndex = getShadowAtlasRegionIndex(atlasManager);
//            }
//
//            populateUniform(uniform, *light, light->views[0]);
//
//            uniformIndex++;
//        }

//    std::cout<<numActiveLights<<std::endl;

    numActiveLights = uniformIndex;
    uniformBuffer.update(0, numActiveLights);
}

void LightsUniformBuffer::populateUniform(LightUniform &uniform, Light &light, LightView &view)
{
    bool hasShadowAtlasIndex = view.shadowAtlasIndex > 0;

    uniform.isPointSource = light.type == Light::SPOT;
    uniform.doesCastShadows = (unsigned int)(light.doesCastShadows && hasShadowAtlasIndex);
    uniform.position = light.position;
    uniform.color = light.color;
    uniform.distAttenMax = light.distAttenMax;
    uniform.distAttenMin = light.distAttenMin;
    uniform.intensity = light.intensity;
    uniform.direction = light.direction;
    uniform.beamAngle = glm::radians(light.beamAngle);
    uniform.projectionViewMatrix = view.projectionViewMatrix;
    uniform.shadowAtlasIndex = view.shadowAtlasIndex;
    uniform.projectionTextureId = light.projectionTextureId;
}

unsigned int LightsUniformBuffer::getShadowAtlasRegionIndex(TextureAtlasManager &atlasManager, unsigned int maxTextureSize)
{
    int index = atlasManager.occupyRegion(TextureAtlasManager::ATLAS_SHADOW_DEPTH, (int)maxTextureSize);
    return index > 0 ? index : 0;
}

void LightsUniformBuffer::bind(Shader &shader)
{
    uniformBuffer.bind();
    shader.setUniform("lights", uniformBuffer.getBufferId());
    shader.setUniform("numActiveLights", numActiveLights);

//    std::cout<<numActiveLights<<std::endl;
}