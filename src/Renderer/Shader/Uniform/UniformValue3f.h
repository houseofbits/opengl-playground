#pragma once

#include "UniformValue.h"
#include <GL/glew.h>
#include <glm/vec3.hpp>

class UniformValue3f final : public UniformValue {
public:
    explicit UniformValue3f(const glm::vec3 &value) : val(value) {
    }

    UniformValue3f(const float x, const float y, const float z) : val(x, y, z) {
    }

    void bind(int location) override {
        glUniform3f(location, val.x, val.y, val.z);
    }

private:
    glm::vec3 val;
};
