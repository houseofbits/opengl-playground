#pragma once

#include "Light.h"
#include "Scene.h"
#include "TextureAtlasManager.h"
#include "UniformBuffer.h"
#include "Shader.h"

class LightsUniformBuffer
{
public:
    typedef UniformBuffer<LightUniform> LightUniformBuffer;

    LightsUniformBuffer();

private:
    LightUniformBuffer uniformBuffer;
    unsigned int maxActiveLights;
    unsigned int numActiveLights;

    void populateUniform(LightUniform &uniform, Light &light, LightView &view);
    unsigned int getShadowAtlasRegionIndex(TextureAtlasManager &atlasManager);

public:
    void create(unsigned int maxLights);
    void update(Scene &scene, TextureAtlasManager &atlasManager);
    void bind(Shader &shader);
    LightUniform &get(unsigned int index)
    {
        return uniformBuffer.get(index);
    }
    unsigned int getNumActiveLights()
    {
        return numActiveLights;
    }
};