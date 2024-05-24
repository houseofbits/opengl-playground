#include "UniformBuffer.h"
#include "../Light/Light.h"
#include <cstring>
#include <glm/vec4.hpp>

template class UniformBuffer<float>;
template class UniformBuffer<unsigned int>;
template class UniformBuffer<glm::vec4>;
template class UniformBuffer<LightUniform>;

template <typename T>
void UniformBuffer<T>::create(unsigned int size, DrawType drawTypeInput, unsigned int bindingIndex)
{
    drawType = drawTypeInput;
    maxSize = size;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * maxSize, NULL, drawType);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (drawType == DYNAMIC)
    {
        data = new T[size];
    }
}

template <typename T>
void UniformBuffer<T>::setData(const T *inputData, unsigned int offset, unsigned int size)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T) * size, inputData);

    if (drawType == DYNAMIC)
    {
        std::memcpy(&data[offset], inputData, sizeof(T) * size);
    }
}

template <typename T>
void UniformBuffer<T>::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
}

template <typename T>
void UniformBuffer<T>::update(unsigned int offset, unsigned int size)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T) * size, data);
}