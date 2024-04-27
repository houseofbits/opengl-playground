#pragma once

#include "RenderTarget.h"
#include <map>

class TextureAtlas : public RenderTarget
{
private:
    std::map<unsigned int, bool> ocupancy;
    unsigned int textureBindingId;

public:
    TextureAtlas &setTextureBindingId(unsigned int id)
    {
        textureBindingId = id;

        return *this;
    }

    TextureAtlas &setSize(unsigned int size)
    {
        width = height = size;

        return *this;
    }

    TextureAtlas &setOccupied(unsigned int index, bool isOccupied)
    {
        ocupancy[index] = isOccupied;

        return *this;
    }

    TextureAtlas &enableAsRenderTarget(RenderTarget::Target target)
    {
        // create(target, width, height);

        return *this;
    }

    TextureAtlas &enableColorTexture()
    {
        // createTexture(RenderTarget::TARGET_COLOR, width, height);

        return *this;
    }

    bool isOccupied(unsigned int index)
    {
        return ocupancy[index];
    }
};