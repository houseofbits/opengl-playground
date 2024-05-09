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
    bool isRenderTarget;

    TextureAtlas();
    ~TextureAtlas();

    int getSize() const;

    TextureAtlas &create(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);
    TextureAtlas &createAsRenderTarget(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);

    void bindRenderTarget() const;
    void unbindRenderTarget() const;
    void bindTexture() const;
};