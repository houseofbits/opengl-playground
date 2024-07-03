#pragma once

#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "../../../ResourcesV2/ShaderProgramResource.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include <glm/vec3.hpp>

struct EnvironmentProbeStructure {
    glm::vec3 position{0};
    unsigned int cubeMapTextureLayer{0};
    glm::vec4 boundingBoxMin{-0.5};
    glm::vec4 boundingBoxMax{0.5};
    glm::vec3 debugColor{1};
    float _PLACEHOLDER0{0};
};

class EnvironmentProbeStorageBuffer : public ShaderStorageBuffer<EnvironmentProbeStructure> {
public:
    inline static const int MAX_PROBES = 100;

    EnvironmentProbeStorageBuffer();

    void initialize();
    void appendProbe(TransformComponent &transform, EnvironmentProbeComponent &probe);
    void bind(ShaderProgramResource &shader);
};
