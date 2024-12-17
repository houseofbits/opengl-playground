#include "EnvironmentProbesBufferResource.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"

EnvironmentProbesBufferResource::EnvironmentProbesBufferResource() : Resource(), m_StorageBuffer() {
}

Resource::Status EnvironmentProbesBufferResource::build() {
    if (!m_StorageBuffer.isSupported()) {
        return STATUS_BUILD_ERROR;
    }

    m_StorageBuffer.create(
            MAX_PROBES,
            ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void EnvironmentProbesBufferResource::destroy() {
}

void EnvironmentProbesBufferResource::bind(ShaderProgramResource &shader) {
    m_StorageBuffer.bind();
    shader.setUniform(getSizeAttributeName().c_str(), m_StorageBuffer.currentSize);
}

void EnvironmentProbesBufferResource::appendProbe(TransformComponent &transform, EnvironmentProbeComponent &probe) {
    EnvironmentProbeStructure structure;
    structure.position = transform.getWorldPosition();
    glm::vec3 size = transform.getScale();
    structure.boundingBoxMin = glm::vec4(-(size * glm::vec3(0.5)) + structure.position, 1.0);
    structure.boundingBoxMax = glm::vec4(size * glm::vec3(0.5) + structure.position, 1.0);
    structure.debugColor = probe.m_DebugColor;
    structure.cubeMapTextureLayer = probe.m_cubeMapLayerIndex;

    m_StorageBuffer.append(structure);
}