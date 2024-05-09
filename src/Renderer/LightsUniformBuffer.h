#pragma once

#include "../Components/Scene.h"
#include "Light.h"
#include "Shader.h"
#include "TextureAtlasManager.h"
#include "UniformBuffer.h"

class LightsUniformBuffer
{
public:
    static const unsigned int LIGHTS_UNIFORM_BINDING_INDEX = 1;

    typedef UniformBuffer<LightUniform> LightUniformBuffer;

    LightsUniformBuffer();

private:
    LightUniformBuffer uniformBuffer;
    unsigned int maxActiveLights;
    unsigned int numActiveLights;

    static void populateUniform(LightUniform &uniform, Light &light, LightView &view);
    static unsigned int getShadowAtlasRegionIndex(TextureAtlasManager &atlasManager);

public:
    void create(unsigned int maxLights);
    void update(Scene &scene, TextureAtlasManager &atlasManager);
    void bind(Shader &shader);
    LightUniform &get(unsigned int index)
    {
        return uniformBuffer.get(index);
    }
    [[nodiscard]] unsigned int getNumActiveLights() const
    {
        return numActiveLights;
    }
};