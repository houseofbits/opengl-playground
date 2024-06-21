#pragma once

#include "../CoreV2/Resources/Resource.h"
#include "../Helper/Log.h"
#include <cstdint>

class TextureResource : public Resource {
public:
    explicit TextureResource();

    void fetchData(ResourceManager&) override;
    void build() override;
    void destroy() override;

    int m_width;
    int m_height;
    unsigned char *m_data;
    unsigned int m_textureId;
    std::uint64_t m_handleId;
};
