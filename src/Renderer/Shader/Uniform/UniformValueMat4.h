#pragma once

#include "UniformValue.h"
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

class UniformValueMat4 final : public UniformValue {
public:
    UniformValueMat4(const glm::mat4& v) : val(v) {
    }

    void bind(int location) override {
        glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
    }

private:
    glm::mat4 val;
};
