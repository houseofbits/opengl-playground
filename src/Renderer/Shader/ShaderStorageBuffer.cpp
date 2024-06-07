#include "ShaderStorageBuffer.h"
#include <cstring>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

template class ShaderStorageBuffer<unsigned int>;
template class ShaderStorageBuffer<float>;
template class ShaderStorageBuffer<glm::vec3>;
template class ShaderStorageBuffer<glm::vec4>;

template<typename T>
void ShaderStorageBuffer<T>::create(unsigned int size, unsigned int bindingIndex) {
    maxSize = size;
    uniformBindingIndex = bindingIndex;

    glCreateBuffers(1, &bufferId);
    glNamedBufferStorage(bufferId,
                         sizeof(T) * maxSize,
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT);

    data = new T[maxSize];
}

template<typename T>
void ShaderStorageBuffer<T>::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, uniformBindingIndex, bufferId);
}

template<typename T>
void ShaderStorageBuffer<T>::setData(const T *inputData, unsigned int offset, unsigned int size) {
    bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T) * size, inputData);

    std::memcpy(&data[offset], inputData, sizeof(T) * size);
}

template<typename T>
void ShaderStorageBuffer<T>::update(unsigned int offset, unsigned int size) {
    bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T) * size, data);
}
