#pragma once

class BaseShaderStorageBuffer 
{
public:
    BaseShaderStorageBuffer();

    virtual ~BaseShaderStorageBuffer();

    virtual void bind() = 0;
};
