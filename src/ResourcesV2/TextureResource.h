#pragma once

#include "../CoreV2/Resources/Resource.h"
#include "../Helper/Log.h"
#include <cstdint>

class TextureResource : public Resource {
public:
    explicit TextureResource();

    void fetchData() override {
        setDataReadyStatus();
    }
    void build() override;
    void destroy() override;


    unsigned int textureId;
    std::uint64_t handleId;
};
