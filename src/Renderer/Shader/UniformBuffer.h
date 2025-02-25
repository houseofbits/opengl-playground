#pragma once

#include <GL/glew.h>

template <typename T>
class UniformBuffer
{
public:
    enum DrawType
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };

public:
    unsigned int maxSize;
    unsigned int bufferId;
    T *data;
    DrawType drawType;
    unsigned int uniformBindingIndex;

public:
    UniformBuffer() : maxSize(0), bufferId(0), data(nullptr), drawType(STATIC), uniformBindingIndex()
    {
    }

    void create(unsigned int maxSize, DrawType drawType, unsigned int bindingIndex);
    void update(unsigned int offset, unsigned int size);
    void setData(const T *data, unsigned int offset, unsigned int size);
    T &get(unsigned int index)
    {
        return data[index];
    }
    void set(T value, unsigned int index)
    {
        if (data != nullptr && index < maxSize)
        {
            data[index] = value;
        }
    }
    void bind();
    unsigned int &getBufferId()
    {
        return bufferId;
    }
};