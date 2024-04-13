#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../libs/tinygltf/stb_image.h"
#include <GL/glew.h>
#include <string>
#include <iostream>

class Texture2D
{
public:
    static unsigned int createTexture(std::string filename)
    {
        int w, h, comp;
        const uint8_t *img = stbi_load(filename.c_str(), &w, &h, &comp, 3);

        if (img == nullptr)
        {
            std::cout << "Failed to load image " << filename << std::endl;

            return 0;
        }

        unsigned int texture;
        // glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(texture, 1, GL_RGB8, w, h);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureSubImage2D(texture, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }
};