#pragma once

#include "UniformValue.h"
#include <GL/glew.h>

class UniformValue1f final : public UniformValue {
public:
    UniformValue1f(float v) : val(v) {
    }

    void bind(int location) override {
        glUniform1f(location, val);
    }

private:
    float val;
};
