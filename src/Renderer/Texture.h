#pragma once

class Texture
{
public:
    enum Type
    {
        TYPE_DEPTH,
        TYPE_RGBA,
    };

    Texture();
    virtual ~Texture();

    unsigned int textureId;
    unsigned int width;
    unsigned int height;
    Type type;

    virtual void create(unsigned int width, unsigned int height, Type type);
    virtual void applyImage(unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData);
    virtual void bind();
    virtual void unbind();
    virtual void destroy();
};