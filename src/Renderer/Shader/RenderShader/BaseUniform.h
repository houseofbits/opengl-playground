#pragma once

template<typename T>
class BaseUniform {
public:
    BaseUniform() = default;

    virtual ~BaseUniform() = default;

    virtual void bind(int location) = 0;

    T value;
};
