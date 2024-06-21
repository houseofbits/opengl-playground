#pragma once
#include <GL/glew.h>
#include <cstring>

template<typename T>
class ShaderStorageBuffer {
public:
    unsigned int maxSize;
    unsigned int currentSize;
    unsigned int bufferId;
    T *data;
    unsigned int uniformBindingIndex;

public:
    ShaderStorageBuffer() : maxSize(0), currentSize(0), bufferId(0), data(nullptr), uniformBindingIndex() {
    }

    void create(unsigned int size, unsigned int bindingIndex) {
        maxSize = size;
        uniformBindingIndex = bindingIndex;

        glCreateBuffers(1, &bufferId);
        glNamedBufferStorage(bufferId,
                             sizeof(T) * maxSize,
                             nullptr,
                             GL_DYNAMIC_STORAGE_BIT);

        data = new T[maxSize];
    }

    void updateAll() {
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T) * currentSize, data);
    }

    void update(unsigned int offset, unsigned int size) {
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T) * size, data);
    }

    void setData(const T *inputData, unsigned int offset, unsigned int size) {
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T) * size, inputData);

        currentSize = size;
        std::memcpy(&data[offset], inputData, sizeof(T) * size);
    }

    T &get(unsigned int index) {
        return data[index];
    }

    void set(T value, unsigned int index) {
        if (data != nullptr && index < maxSize) {
            data[index] = value;
            if (index > currentSize) {
                currentSize = index;
            }
        }
    }

    void append(T value) {
        if (data != nullptr && currentSize < maxSize) {
            data[currentSize] = value;
            currentSize++;
        }
    }

    void bind() {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, uniformBindingIndex, bufferId);
    }

    unsigned int &getBufferId() {
        return bufferId;
    }

    unsigned int getSize() {
        return currentSize;
    }
};
