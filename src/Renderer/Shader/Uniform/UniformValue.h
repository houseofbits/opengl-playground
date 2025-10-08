#pragma once

class UniformValue {
public:
    UniformValue() {
    }

    virtual ~UniformValue() = default;

    virtual void bind(int location) = 0;
};
