#include "UniformBuffer.h"
#include <glm/vec4.hpp>

template class UniformBuffer<float>;
template class UniformBuffer<glm::vec4>;

template <typename T>
void UniformBuffer<T>::create(unsigned int size, DrawType drawType)
{
    maxSize = size;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * maxSize, NULL, drawType);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T>
void UniformBuffer<T>::setData(const T *data, unsigned int offset, unsigned int size)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T) * size, data);
}

template <typename T>
void UniformBuffer<T>::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
}