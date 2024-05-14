#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include "../../libs/tinygltf/stb_image.h"

Texture TextureLoader::load(std::string filename, bool withMipmapping)
{
    unsigned char *data = nullptr;
    int width, height;
    Texture texture;

    if (loadData(filename, &width, &height, data))
    {
        texture.create(width, height, Texture::TYPE_RGBA);
        if (withMipmapping) {
            texture.enableMipMaps();
        }
        texture.applyImage(0, 0, width, height, data);
    }

    return texture;
}

bool TextureLoader::loadData(std::string filename, int *width, int *height, unsigned char *&data)
{
    int comp;
    data = stbi_load(filename.c_str(), width, height, &comp, 3);

    return data != nullptr;
}
