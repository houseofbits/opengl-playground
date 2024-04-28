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
    virtual void bind();
    virtual void unbind();
    virtual void destroy();
};