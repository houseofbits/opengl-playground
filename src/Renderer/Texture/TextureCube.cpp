
#include "TextureCube.h"
#include <GL/glew.h>
#include <vector>

TextureCube::TextureCube() : Texture()
{

}

void TextureCube::create(unsigned int width, unsigned int height, Type type)
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<unsigned char> testData(width * height * 256, 128);

    for(int side = 0; side < 6; ++side)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                     0,
                     GL_RGBA8,
                     (int)width,
                     (int)height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     &testData[0]);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::applyImageToFace(unsigned int face, unsigned int left, unsigned int top, unsigned int imageWidth, unsigned int imageHeight, const unsigned char *imageData)
{
    bind();
//    glTextureStorage2D(textureId, 5, GL_RGB8, (int) width, (int) height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB8, (int) imageWidth, (int) imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    if (isMipmapsEnabled) {
//        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCube::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

void TextureCube::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}