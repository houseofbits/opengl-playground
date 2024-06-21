#include "TextureResource.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../libs/tinygltf/stb_image.h"

TextureResource::TextureResource() : Resource(), m_textureId(), m_handleId(), m_width(0), m_height(0), m_data(nullptr) {
}

void TextureResource::fetchData(ResourceManager&) {
    int comp;
    m_data = stbi_load(m_Path.c_str(), &m_width, &m_height, &comp, 3);

    if (m_data == nullptr) {
        setFetchErrorStatus();
        return;
    }
    
    setDataReadyStatus();
}

void TextureResource::build() {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    int maxMipMapLevels = 7;

    glTextureParameteri(m_textureId, GL_TEXTURE_MAX_LEVEL, maxMipMapLevels);
    glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_textureId, maxMipMapLevels, GL_RGB8, m_width, m_height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(m_data);
    m_data = nullptr;

    m_handleId = glGetTextureHandleARB(m_textureId);
    if (m_handleId == 0) {
        setBuildErrorStatus();

        std::cout << "Error! Handle returned null " << m_Path << std::endl;

        return;
    }

    glMakeTextureHandleResidentARB(m_handleId);

    setReadyStatus();
}

void TextureResource::destroy() {
    glMakeTextureHandleNonResidentARB(m_handleId);
    glDeleteTextures(1, &m_textureId);
}
