#pragma once

#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include <GL/glew.h>

class ProjectionSamplerStorageBuffer : public ShaderStorageBuffer<GLuint64> {
public:
    inline static const int MAX_PROJECTORS = 100;

    ProjectionSamplerStorageBuffer();

    void initialize();
    int getOrAppend(GLuint64 handleId);
};
