#pragma once

#include "../../../Core/API.h"
#include "../../../Core/Helper/Log.h"
#include <cstdint>

class TextureResource : public Resource {
public:
    explicit TextureResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    int m_width;
    int m_height;
    unsigned char *m_data;
    unsigned int m_textureId;
    std::uint64_t m_handleId;
};
