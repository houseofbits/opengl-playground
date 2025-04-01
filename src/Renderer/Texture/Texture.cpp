#include "Texture.h"
#include <GL/glew.h>
#include <algorithm>
#include <cmath>
#include <iostream>

Texture::Texture() : textureId(0), isMipmapsEnabled(false), width(0), height(0), type(TYPE_RGBA), arrayDepth(0) {
}

Texture::~Texture() = default;

void Texture::create(unsigned int textureWidth, unsigned int textureHeight, Type textureType) {
    type = textureType;
    width = textureWidth;
    height = textureHeight;
    arrayDepth = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (type == TYPE_DEPTH) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (int) width, (int) height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else if (type == TYPE_RGBA) {
        int maxLevels = 7;//1 + (int)floor(log2(std::max(width, height))); //TODO: What should be the right thing to do?

        glTextureParameteri(textureId, GL_TEXTURE_MAX_LEVEL, maxMipMapLevels);
        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(textureId, maxLevels, GL_RGB8, (int) width, (int) height);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else if (type == TYPE_RGBA16) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (int) width, (int) height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else if (type == TYPE_RGBA32) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (int) width, (int) height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void Texture::applyImage(unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData) {
    bind();
    glTextureSubImage2D(textureId, 0, (int) left, (int) top, (int) imageWidth, (int) imageHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    if (isMipmapsEnabled) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::applyTexture(const Texture &texture, unsigned int left, unsigned int top) {
    glCopyImageSubData(texture.textureId, GL_TEXTURE_2D, 1, 0, 0, 0,
                       textureId, GL_TEXTURE_2D, 0, (int) left, (int) top, 0,
                       (int) texture.width, (int) texture.height, 1);

    if (isMipmapsEnabled) {
        int srcWidth = (int) texture.width;
        int srcHeight = (int) texture.height;
        int srcTop = (int) top;
        int srcLeft = (int) left;
        for (int level = 0; level < maxMipMapLevels; ++level) {
            glCopyImageSubData(texture.textureId, GL_TEXTURE_2D, level, 0, 0, 0,
                               textureId, GL_TEXTURE_2D, level, srcLeft, srcTop, 0,
                               srcWidth, srcHeight, 1);

            srcWidth = srcWidth / 2;
            srcHeight = srcHeight / 2;
            srcLeft = srcLeft / 2;
            srcTop = srcTop / 2;
        }
    }
}

void Texture::enableMipMaps() {
    isMipmapsEnabled = true;
    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::destroy() {
    if (glIsTexture(textureId)) {
        glDeleteTextures(1, &textureId);
    }
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::isLoaded() {
    return glIsTexture(textureId);
}
void Texture::createArray(unsigned int textureWidth, unsigned int textureHeight, Type textureType, int depth) {
    type = textureType;
    width = textureWidth;
    height = textureHeight;
    arrayDepth = depth;

    //TODO: Texture array
}
