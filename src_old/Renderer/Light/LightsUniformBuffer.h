#pragma once

#include "../../Components/Scene.h"
#include "../Shader/Shader.h"
#include "../Shader/UniformBuffer.h"
#include "../TextureAtlas/TextureAtlasManager.h"
#include "Light.h"

class LightsUniformBuffer
{
public:
    typedef UniformBuffer<LightUniform> LightUniformBuffer;

    LightsUniformBuffer();

private:
    LightUniformBuffer uniformBuffer;
    unsigned int maxActiveLights;
    unsigned int numActiveLights;

    static void populateUniform(LightUniform &uniform, Light &light, LightView &view);
    static unsigned int getShadowAtlasRegionIndex(TextureAtlasManager &atlasManager, unsigned int maxTextureSize);

public:
    void create(unsigned int maxLights, unsigned int uniformBindingIndex);
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