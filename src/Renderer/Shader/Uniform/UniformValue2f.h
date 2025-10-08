#pragma once

#include "UniformValue.h"
#include <GL/glew.h>
#include <glm/vec2.hpp>

class UniformValue2f final : public UniformValue {
public:
    explicit UniformValue2f(const glm::vec2& value) : val(value) {
    }

    UniformValue2f(const float x, const float y) : val(x, y) {
    }

    void bind(int location) override {
        glUniform2f(location, val.x, val.y);
    }

private:
    glm::vec2 val;
};
