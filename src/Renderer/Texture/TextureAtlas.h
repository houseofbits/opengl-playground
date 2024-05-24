#pragma once

#include "../../Helper/QuadTree.h"
#include "../../Helper/QuadTreeOccupancy.h"
#include "../Shader/Shader.h"
#include "Texture.h"
#include "TextureRenderTarget.h"
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

    TextureAtlas &create(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType, bool withMipmapping = false);
    TextureAtlas &createAsRenderTarget(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType);

    void bindRenderTarget() const;
    void unbindRenderTarget() const;
    void bindTexture() const;
};