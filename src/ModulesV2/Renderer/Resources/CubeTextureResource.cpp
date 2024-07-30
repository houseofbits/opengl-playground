#include "CubeTextureResource.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../../SourceLibs/tinygltf/stb_image.h"
#include "../../../Helper/StringUtils.h"

CubeTextureResource::CubeTextureResource() : Resource(), m_textureId(), m_handleId(), m_width(0), m_height(0), m_data() {
}

Resource::Status CubeTextureResource::fetchData(ResourceManager &) {

    for (unsigned int faceId = 0; faceId < 6; faceId++) {
        int width, height, comp;
        std::string filename = StringUtils::replaceAll(m_Path, CUBE_FACE_FILENAME_PLACEHOLDER, CUBE_FACE_FILENAME_SUFFIXES[faceId]);
        m_data[faceId] = stbi_load(filename.c_str(), &width, &height, &comp, 3);

        if (m_data[faceId] == nullptr) {
            for (int i = 0; i < faceId; ++i) {
                stbi_image_free(m_data[i]);
            }

            return STATUS_FETCH_ERROR;
        }

        m_width = width;
        m_height = height;
    }

    return STATUS_DATA_READY;
}

Resource::Status CubeTextureResource::build() {
    if (!GLEW_ARB_bindless_texture) {
        return STATUS_BUILD_ERROR;
    }

    if (!GLEW_ARB_direct_state_access) {
        return STATUS_BUILD_ERROR;
    }

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glGenTextures(1, &m_textureId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int faceId = 0; faceId < 6; faceId++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceId, 0, GL_RGB8, (int) m_width, (int) m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data[faceId]);
        stbi_image_free(m_data[faceId]);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    if (!glIsTexture(m_textureId)) {
        destroy();

        return STATUS_BUILD_ERROR;
    }

    m_handleId = glGetTextureHandleARB(m_textureId);
    if (m_handleId == 0) {
        Log::error("Error! Handle returned null " + m_Path);

        destroy();

        return STATUS_BUILD_ERROR;
    }

    glMakeTextureHandleResidentARB(m_handleId);

    return STATUS_READY;
}

void CubeTextureResource::destroy() {
    if (m_handleId > 0) {
        glMakeTextureHandleNonResidentARB(m_handleId);
    }
    if (glIsTexture(m_textureId)) {
        glDeleteTextures(1, &m_textureId);
    }
}
