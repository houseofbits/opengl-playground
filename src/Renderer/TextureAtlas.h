#pragma once

#include "Texture.h"
#include "TextureRenderTarget.h"
#include "../Helper/QuadTreeOccupancy.h"
#include <map>

class TextureAtlas
{
private:
    unsigned int textureBinding;
    QuadTreeOccupancy qtOccupancy;
    Texture *texture;

public:
    TextureAtlas();
    ~TextureAtlas();

    TextureAtlas &setTextureBinding(unsigned int id);
    TextureAtlas &create(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);
    TextureAtlas &createAsRenderTarget(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);
};