#include "EnvironmentProbeStorageBuffer.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"

EnvironmentProbeStorageBuffer::EnvironmentProbeStorageBuffer() : ShaderStorageBuffer<EnvironmentProbeStructure>() {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("EnvironmentProbeStorageBuffer");
}

void EnvironmentProbeStorageBuffer::initialize() {
    create(MAX_PROBES, uniformBindingIndex);
}

void EnvironmentProbeStorageBuffer::appendProbe(TransformComponent &transform, EnvironmentProbeComponent &probe) {
    EnvironmentProbeStructure structure;
    structure.position = transform.getWorldPosition();
    glm::vec3 size = transform.getScale();
    structure.boundingBoxMin = glm::vec4(-(size * glm::vec3(0.5)) + structure.position, 1.0);
    structure.boundingBoxMax = glm::vec4(size * glm::vec3(0.5) + structure.position, 1.0);
    structure.debugColor = probe.m_DebugColor;

    append(structure);
}

void EnvironmentProbeStorageBuffer::bind(ShaderProgramResource &shader) {
    ShaderStorageBuffer<EnvironmentProbeStructure>::bind();
    shader.setUniform("numEnvProbes", currentSize);
}
