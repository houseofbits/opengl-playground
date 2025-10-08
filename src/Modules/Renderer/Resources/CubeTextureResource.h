#pragma once

#include "ShaderUniformResource.h"
#include "../../../Core/API.h"
#include "../../../Core/Helper/Log.h"
#include <cstdint>

class CubeTextureResource : public ShaderUniformResource {
public:
    inline static const std::string CUBE_FACE_FILENAME_PLACEHOLDER = "*";
    inline static const std::string CUBE_FACE_FILENAME_SUFFIXES[6] = {
            "px",
            "nx",
            "py",
            "ny",
            "pz",
            "nz",
    };

    explicit CubeTextureResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    void use(Shader &shader, const std::string &uniformName) override;

    int m_width;
    int m_height;
    unsigned char *m_data[6];
    unsigned int m_textureId;
    std::uint64_t m_handleId;
};
