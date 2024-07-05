#pragma once

#include "../../../CoreV2/API.h"

class LightsBufferResource : public Resource {
public:
    LightsBufferResource();

    Resource::Status build() override;
    void destroy() override;
};
