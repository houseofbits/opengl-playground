#include "LightStorageBuffer.h"
#include "../../../Helper/ShaderSourceLoader.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

template class ShaderStorageBuffer<SpotLightStructure>;

LightStorageBuffer::LightStorageBuffer() : ShaderStorageBuffer<SpotLightStructure>(0) {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("SpotLightStorageBuffer");
}

void LightStorageBuffer::initialize() {
    create(MAX_SPOT_LIGHTS, uniformBindingIndex);
}

void LightStorageBuffer::appendLight(TransformComponent &transform, LightComponent &light, int projectorIndex) {
    SpotLightStructure structure;
    if (light.m_Type == LightComponent::SPOT) {
        structure.projectionViewMatrix = createPerspectiveProjectionViewMatrix(transform, light);
        structure.color = light.m_Color;
        structure.intensity = light.m_Intensity;
        structure.position = transform.getTranslation();
        structure.direction = transform.getDirection();
        structure.attenuation = light.m_Attenuation;
        structure.projectorSamplerIndex = projectorIndex;
        structure.isPointSource = 1;

        append(structure);
    } else if (light.m_Type == LightComponent::DIRECT) {
        structure.projectionViewMatrix = createOrthoProjectionViewMatrix(transform, light);
        structure.color = light.m_Color;
        structure.intensity = light.m_Intensity;
        structure.position = transform.getTranslation();
        structure.direction = transform.getDirection();
        structure.attenuation = light.m_Attenuation;
        structure.projectorSamplerIndex = projectorIndex;
        structure.isPointSource = 0;

        append(structure);
    }

    //TODO
}

void LightStorageBuffer::bind(ShaderProgramResource &shader) {
    ShaderStorageBuffer<SpotLightStructure>::bind();
    shader.setUniform("numSpotLights", currentSize);
}

glm::mat4 LightStorageBuffer::createPerspectiveProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {

    glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(light.m_beamAngle), 1.0, 0.01, light.m_Attenuation);

    return projectionMatrix * transform.getInverseModelMatrix();
}

glm::mat4 LightStorageBuffer::createOrthoProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {
    glm::mat4 projectionMatrix = glm::ortho<float>(-light.m_Radius, light.m_Radius, -light.m_Radius, light.m_Radius, 0.01, light.m_Attenuation);

    return projectionMatrix * transform.getInverseModelMatrix();
}
