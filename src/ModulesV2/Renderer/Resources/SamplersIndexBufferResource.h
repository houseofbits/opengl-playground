#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "ShaderProgramResource.h"

class SamplersIndexBufferResource : public Resource {
public:
    inline static const int MAX_SAMPLERS = 100;

    SamplersIndexBufferResource();

    Resource::Status build() override;
    void destroy() override;
    void bind(ShaderProgramResource &shader);
    int getOrAppend(GLuint64 handleId);

    ShaderStorageBuffer<GLuint64> m_StorageBuffer;
};
