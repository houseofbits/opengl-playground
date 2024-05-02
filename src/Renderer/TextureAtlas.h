#pragma once

#include "Texture.h"
#include "TextureRenderTarget.h"
#include "Shader.h"
#include "../Helper/QuadTreeOccupancy.h"
#include "../Helper/QuadTree.h"
#include <map>
#include <string>

class TextureAtlas
{
public:
    QuadTreeOccupancy qtOccupancy;
    Texture *texture;

    TextureAtlas();
    ~TextureAtlas();

    int getSize();

    TextureAtlas &create(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);
    TextureAtlas &createAsRenderTarget(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);
};