#pragma once

#include "../../../Core/API.h"

class Shader;

class ShaderUniformResource : public Resource {
public:
    virtual void use(Shader* shader) { }
};
