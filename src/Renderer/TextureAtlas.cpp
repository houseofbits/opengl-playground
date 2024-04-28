#include "TextureAtlas.h"

TextureAtlas::TextureAtlas() : textureBinding(0), texture(nullptr), qtOccupancy()
{
}

TextureAtlas::~TextureAtlas()
{
}

TextureAtlas &TextureAtlas::setTextureBinding(unsigned int id)
{
    textureBinding = id;

    return *this;
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
