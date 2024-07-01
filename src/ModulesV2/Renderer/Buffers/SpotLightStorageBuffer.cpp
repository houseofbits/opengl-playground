#include "SpotLightStorageBuffer.h"
#include "../../../Helper/ShaderSourceLoader.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

template class ShaderStorageBuffer<SpotLightStructure>;

SpotLightStorageBuffer::SpotLightStorageBuffer() : ShaderStorageBuffer<SpotLightStructure>(0) {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("SpotLightStorageBuffer");
}

void SpotLightStorageBuffer::initialize() {
    create(MAX_SPOT_LIGHTS, uniformBindingIndex);
}

void SpotLightStorageBuffer::appendLight(TransformComponent &transform, LightComponent &light, int projectorIndex) {
    SpotLightStructure structure;
    structure.projectionViewMatrix = createProjectionViewMatrix(transform, light);
    structure.color = light.m_Color;
    structure.intensity = light.m_Intensity;
    structure.position = transform.getTranslation();
    structure.attenuation = light.m_Attenuation;
    structure.projectorSamplerIndex = projectorIndex;

    append(structure);
}

void SpotLightStorageBuffer::bind(ShaderProgramResource &shader) {
    ShaderStorageBuffer<SpotLightStructure>::bind();
    shader.setUniform("numSpotLights", currentSize);
}

glm::mat4 SpotLightStorageBuffer::createProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {

    glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(light.m_beamAngle), 1.0, 0.01, light.m_Attenuation);

    return projectionMatrix * transform.getInverseModelMatrix();
}
