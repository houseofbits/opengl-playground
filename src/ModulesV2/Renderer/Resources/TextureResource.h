#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Helper/Log.h"
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
