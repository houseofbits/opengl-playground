#include "TextureLoader.h"
#include "../../Helper/StringUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include "../../SourceLibs/tinygltf/stb_image.h"

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
        freeData(data);
    }

    return texture;
}

bool TextureLoader::loadData(std::string filename, int *width, int *height, unsigned char *&data)
{
    int comp;
    data = stbi_load(filename.c_str(), width, height, &comp, 3);

    return data != nullptr;
}

TextureCube TextureLoader::loadCube(std::string imageFileName, bool withMipmapping)
{
    TextureCube texture;
    bool isTextureCreated = false;

    int width, height, prevWidth = 0, prevHeight = 0;
    for (unsigned int faceId = 0; faceId < 6; faceId++)
    {
        unsigned char *data = nullptr;
        std::string filename = StringUtils::replaceAll(imageFileName, CUBE_FACE_FILENAME_PLACEHOLDER, CUBE_FACE_FILENAME_SUFFIXES[faceId]);
        if (loadData(filename, &width, &height, data)) {
            if (!isTextureCreated) {
                texture.create(width, height, Texture::TYPE_RGBA);
                isTextureCreated = true;
            }
            //            if (prevWidth != 0 && prevWidth != width)
            //            {
            //                std::cout<<"Cube map texture size differs from rest of the faces"<<filename<<std::endl;
            //                areAllFacesLoaded = false;
            //                break;
            //            }
            //            if (prevHeight != 0 && prevHeight != height)
            //            {
            //                std::cout<<"Cube map texture size differs from rest of the faces"<<filename<<std::endl;
            //                areAllFacesLoaded = false;
            //                break;
            //            }
            //
            //            prevWidth = width;
            //            prevHeight = height;
            //        } else {
            //            std::cout<<"Cube map face texture not found"<<filename<<std::endl;
            //            areAllFacesLoaded = false;
            //            break;
            //        }
//            std::cout<<"load data "<<filename<<std::endl;
            texture.applyImageToFace(faceId, 0, 0, width, height, data);
            freeData(data);
        } else {
            std::cout<<"Failed to load "<<filename<<std::endl;
        }
    }

    texture.bind();
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return texture;
}

void TextureLoader::freeData(void *data)
{
    stbi_image_free(data);
}