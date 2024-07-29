#pragma once

class Texture
{
public:
    enum Type
    {
        TYPE_DEPTH,
        TYPE_RGBA,
        TYPE_RGBA16,
        TYPE_RGBA32,
    };

    static const int maxMipMapLevels = 7;

    Texture();
    virtual ~Texture();

    unsigned int textureId;
    unsigned int width;
    unsigned int height;
    unsigned int arrayDepth;
    Type type;
    bool isMipmapsEnabled;

    virtual void create(unsigned int width, unsigned int height, Type type);
    virtual void createArray(unsigned int width, unsigned int height, Type type, int depth);
    void enableMipMaps();
    void applyImage(unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData);
    void applyTexture(const Texture& texture, unsigned int left, unsigned int top);

    virtual void bind();
    virtual void unbind();
    virtual void destroy();

    bool isLoaded();
};