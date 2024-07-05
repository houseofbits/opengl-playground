#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"

class SamplersIndexBufferResource : public Resource {
public:
    inline static const int MAX_SAMPLERS = 100;

    SamplersIndexBufferResource();

    Resource::Status build() override;
    void destroy() override;

    ShaderStorageBuffer<GLuint64> m_StorageBuffer;
};
