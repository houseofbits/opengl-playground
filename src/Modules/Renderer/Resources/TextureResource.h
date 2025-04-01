#pragma once

#include "../../../Core/API.h"
#include "../../../Core/Helper/Log.h"
#include <cstdint>
#include "ShaderProgramResource.h"


class TextureResource : public Resource {
public:
    explicit TextureResource();

    Status fetchData(ResourceManager&) override;
    Status build() override;
    void destroy() override;

    void bind(ShaderProgramResource &shader, int unit = 0) const;
    void bindImageTexture(unsigned int unit) const;

    int m_width;
    int m_height;
    unsigned char *m_data;
    unsigned int m_textureId;
    std::uint64_t m_handleId;
};
