#include "TextureResource.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../libs/tinygltf/stb_image.h"

TextureResource::TextureResource() = default;

void TextureResource::build() {

    unsigned char *data = nullptr;
    int width, height;
    int comp;
    data = stbi_load(m_Path.c_str(), &width, &height, &comp, 3);

    if (data == nullptr) {
        setFetchErrorStatus();
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    int maxMipMapLevels = 7;

    glTextureParameteri(textureId, GL_TEXTURE_MAX_LEVEL, maxMipMapLevels);
    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(textureId, maxMipMapLevels, GL_RGB8, (int) width, (int) height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureSubImage2D(textureId, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    handleId = glGetTextureHandleARB(textureId);
    if (handleId == 0) {
        setBuildErrorStatus();

        std::cout << "Error! Handle returned null " << m_Path << std::endl;

        return;
    }

    glMakeTextureHandleResidentARB(handleId);

    setReadyStatus();
}

void TextureResource::destroy() {
    glMakeTextureHandleNonResidentARB(handleId);
    glDeleteTextures(1, &textureId);
}
