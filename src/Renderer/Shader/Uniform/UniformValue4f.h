#pragma once

#include "UniformValue.h"
#include <GL/glew.h>
#include <glm/vec4.hpp>

class UniformValue4f final : public UniformValue {
public:
    explicit UniformValue4f(const glm::vec4 &value) : val(value) {
    }

    UniformValue4f(float x, float y, float z, float w) : val(x, y, z, w) {
    }

    void bind(int location) override {
        glUniform4f(location, val.x, val.y, val.z, val.w);
    }

private:
    glm::vec4 val;
};
