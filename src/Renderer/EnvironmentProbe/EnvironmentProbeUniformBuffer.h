#pragma once

#include "../../Components/Scene.h"
#include "../Shader/UniformBuffer.h"
#include "EnvironmentProbe.h"

class EnvironmentProbeUniformBuffer {
public:
    typedef UniformBuffer<EnvironmentProbeUniform> EnvironmentProbeUniformBufferType;

    EnvironmentProbeUniformBuffer();

    EnvironmentProbeUniformBufferType uniformBuffer;
    unsigned int maxActiveProbes;
    unsigned int numActiveProbes;

    void create(unsigned int maxProbes, unsigned int uniformBindingIndex);
    void update(Scene &scene);
    void bind(Shader &shader);
};
