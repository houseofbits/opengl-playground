#pragma once

class BaseShaderStorageBuffer 
{
public:
    BaseShaderStorageBuffer() = default;

    virtual ~BaseShaderStorageBuffer() = default;

    virtual void bind() = 0;
};
