#include "TextureAtlas.h"
#include "TextureLoader.h"
#include <assert.h>

TextureAtlas::TextureAtlas() : texture(nullptr), qtOccupancy()
{
}

TextureAtlas::~TextureAtlas()
{
}

int TextureAtlas::getSize()
{
    assert(texture != nullptr);
    assert(texture->width == texture->height);

    return texture->width;
}

TextureAtlas &TextureAtlas::create(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType)
{
    texture = new Texture();
    texture->create(textureWidth, textureHeight, textureType);

    return *this;
}

TextureAtlas &TextureAtlas::createAsRenderTarget(unsigned int textureWidth, unsigned int textureHeight, Texture::Type textureType)
{
    texture = new TextureRenderTarget();
    texture->create(textureWidth, textureHeight, textureType);

    return *this;
}
