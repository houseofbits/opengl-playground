#include "TextureResource.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "ShaderResource.h"
#include "../../../../libs/tinygltf/stb_image.h"

TextureResource::TextureResource() : Resource(), m_textureId(), m_handleId(), m_width(0), m_height(0), m_data(nullptr) {
}

Resource::Status TextureResource::fetchData(ResourceManager &) {
    int comp;
    m_data = stbi_load(m_Path.c_str(), &m_width, &m_height, &comp, 4);

    if (m_data == nullptr) {
        return STATUS_FETCH_ERROR;
    }

    return STATUS_DATA_READY;
}

Resource::Status TextureResource::build() {
    if (!GLEW_ARB_bindless_texture) {
        Log::error("Texture resource requires bindless textures extension.");

        stbi_image_free(m_data);
        m_data = nullptr;

        return STATUS_BUILD_ERROR;
    }

    if (!GLEW_ARB_direct_state_access) {
        Log::error("Texture resource requires direct state access extension.");

        stbi_image_free(m_data);
        m_data = nullptr;

        return STATUS_BUILD_ERROR;
    }

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    int maxMipMapLevels = 7;

    //Direct state access 4.5 or ARB_direct_state_access
    glTextureParameteri(m_textureId, GL_TEXTURE_MAX_LEVEL, maxMipMapLevels);
    glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_textureId, maxMipMapLevels, GL_RGBA8, m_width, m_height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(m_data);
    m_data = nullptr;

    m_handleId = glGetTextureHandleARB(m_textureId);
    if (m_handleId == 0) {
        std::cout << "Error! Handle returned null " << m_Path << std::endl;

        return STATUS_BUILD_ERROR;
    }

    glMakeTextureHandleResidentARB(m_handleId);

    return STATUS_READY;
}

void TextureResource::destroy() {
    if (m_handleId > 0) {
        glMakeTextureHandleNonResidentARB(m_handleId);
    }
    if (glIsTexture(m_textureId)) {
        glDeleteTextures(1, &m_textureId);
    }
}

void TextureResource::bind(ShaderProgramResource &shader, const int unit) const {
    shader.setUniform("screenTexture", unit);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void TextureResource::bindImageTexture(const unsigned int unit) const {
    glBindImageTexture(unit, m_textureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
}

