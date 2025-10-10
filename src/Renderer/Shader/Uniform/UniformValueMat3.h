#pragma once

#include "UniformValue.h"
#include <GL/glew.h>
#include <glm/mat3x3.hpp>

class UniformValueMat3 final : public UniformValue {
public:
    explicit UniformValueMat3(const glm::mat3& v) : val(v) {
    }

    void bind(int location) override {
        glUniformMatrix3fv(location, 1, GL_FALSE, &val[0][0]);
    }

private:
    glm::mat3 val;
};
