#pragma once

#include "../../../Core/API.h"

class Shader;

class ShaderUniformResource : public Resource {
public:
    ShaderUniformResource() = default;

    ~ShaderUniformResource() override = default;

    virtual void use(Shader &shader) {
    }

    virtual void use(Shader &shader, const std::string &uniformName) {
    }
};
