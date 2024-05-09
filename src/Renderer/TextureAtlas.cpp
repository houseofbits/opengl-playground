#include "TextureAtlas.h"
#include "TextureLoader.h"
#include <assert.h>

TextureAtlas::TextureAtlas() : texture(nullptr), qtOccupancy(), isRenderTarget(false)
{
}

TextureAtlas::~TextureAtlas() = default;

int TextureAtlas::getSize() const
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
    isRenderTarget = true;

    return *this;
}

void TextureAtlas::bindRenderTarget() const
{
    if (!isRenderTarget)
    {
        std::cout << "TextureAtlas can not be used as render target" << std::endl;
    }
    texture->bind();
}

void TextureAtlas::unbindRenderTarget() const
{
    texture->unbind();
}

void TextureAtlas::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
}