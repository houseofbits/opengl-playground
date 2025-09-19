#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/Shader.h"

class ShaderProgramResource final : public Resource, public Shader {
public:
    ShaderProgramResource();

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override;
};
