#pragma once

#include "UniformValue.h"
#include <GL/glew.h>

class UniformValue64i final : public UniformValue {
public:
    UniformValue64i(const uint64_t v) : val(v) {
    }

    void bind(int location) override {
        glUniformHandleui64ARB(location, val);
    }

private:
    uint64_t val;
};
