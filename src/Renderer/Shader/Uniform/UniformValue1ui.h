#pragma once

#include "UniformValue.h"
#include <GL/glew.h>

class UniformValue1ui final : public UniformValue {
public:
    explicit UniformValue1ui(unsigned int v) : val(v) {
    }

    void bind(int location) override {
        glUniform1ui(location, val);
    }

private:
    unsigned int val;
};
