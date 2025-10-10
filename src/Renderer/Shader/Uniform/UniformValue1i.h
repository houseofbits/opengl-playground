#pragma once

#include "UniformValue.h"
#include <GL/glew.h>

class UniformValue1i final : public UniformValue {
public:
    explicit UniformValue1i(int v) : val(v) {
    }

    void bind(int location) override {
        glUniform1i(location, val);
    }

private:
    int val;
};
