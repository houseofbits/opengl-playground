#pragma once

#include <GL/glew.h>

template <typename T>
class UniformBuffer
{
private:
    unsigned int maxSize;
    unsigned int bufferId;

public:
    enum DrawType
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };

    void create(unsigned int maxSize, DrawType drawType);
    void setData(const T *data, unsigned int offset, unsigned int size);
    void bind();
    unsigned int &getBufferId()
    {
        return bufferId;
    }
};