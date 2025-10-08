#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/RenderShader.h"

class RenderShaderResource : public Resource {
public:
    RenderShaderResource() = default;

    ~RenderShaderResource() override = default;

    virtual void applyToShader(RenderShader &shader) {
    }

    virtual void applyToShader(RenderShader &shader, const std::string &uniformName) {
    }
};
