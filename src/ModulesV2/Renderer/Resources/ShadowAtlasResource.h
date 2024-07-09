#pragma once

#include "../../../CoreV2/API.h"
#include "ShaderProgramResource.h"

class ShadowAtlasResource : public Resource {
public:
    ShadowAtlasResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    void bind(ShaderProgramResource &shader);
};
