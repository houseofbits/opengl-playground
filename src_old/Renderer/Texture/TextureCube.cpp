
#include "TextureCube.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>

TextureCube::TextureCube() : Texture() {
}

void TextureCube::create(unsigned int w, unsigned int h, Type t) {
    type = t;
    width = w;
    height = h;
    arrayDepth = 0;

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glGenTextures(1, &textureId);

    bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    fillWithTemporaryData();

    unbind();
}

void TextureCube::createArray(unsigned int w, unsigned int h, Type t, int depth) {
    type = t;
    width = w;
    height = h;
    arrayDepth = depth;

    type = t;
    width = w;
    height = h;

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glGenTextures(1, &textureId);

    bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY,
                   7,
                   GL_RGB8,
                   (int) width, (int) height,
                   (int) arrayDepth);

    fillWithTemporaryData();

    unbind();
}

void TextureCube::applyImageToFace(unsigned int face, unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData) {
    bind();
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB8, (int) imageWidth, (int) imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    if (isMipmapsEnabled) {
        //        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCube::bind() {
    if (arrayDepth > 0) {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, textureId);
    } else {
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    }
}

void TextureCube::unbind() {
    if (arrayDepth > 0) {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
    } else {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

void TextureCube::fillWithTemporaryData() {
    std::vector<unsigned char> testData(width * height * 256, 128);

    if (arrayDepth > 0) {
        glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY,
                     0,
                     GL_RGBA8,
                     (int) width,
                     (int) height,
                     (int) arrayDepth * 6,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     nullptr);

        for (int layer = 0; layer < arrayDepth; ++layer) {
            for (int side = 0; side < 6; ++side) {
                int zOffset = layer * 6 + side;
                glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY,
                                0,
                                0, 0, zOffset,
                                (int) width, (int) height, 1,
                                GL_RGBA,
                                GL_UNSIGNED_BYTE,
                                &testData[0]);
            }
        }
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
    } else {
        for (int side = 0; side < 6; ++side) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                         0,
                         GL_RGBA8,
                         (int) width,
                         (int) height,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         &testData[0]);
        }
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}
