#include "SpotLightStorageBuffer.h"
#include "../../../Helper/ShaderSourceLoader.h"

template class ShaderStorageBuffer<SpotLightStructure>;

SpotLightStorageBuffer::SpotLightStorageBuffer() : ShaderStorageBuffer<SpotLightStructure>() {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("SpotLightStorageBuffer");
}

void SpotLightStorageBuffer::initialize() {
    create(MAX_SPOT_LIGHTS, uniformBindingIndex);
}

void SpotLightStorageBuffer::appendLight(TransformComponent &transform, LightComponent &light) {
    SpotLightStructure structure;
    structure.projectionViewMatrix = createProjectionViewMatrix(transform, light);
    structure.color = light.m_Color;
    structure.intensity = light.m_Intensity;
//    structure.attenuation = light.m_Attenuation;

//    std::cout<<structure.color.x<<", "<<structure.color.y<<", "<<structure.color.z<<std::endl;

    append(structure);
}

void SpotLightStorageBuffer::bind(ShaderProgramResource &shader) {
    ShaderStorageBuffer<SpotLightStructure>::bind();
    shader.setUniform("numSpotLights", currentSize);
}

glm::mat4 SpotLightStorageBuffer::createProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {

    glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(light.m_beamAngle), 1.0, 0.01, light.m_Attenuation);
    glm::mat4 viewMatrix = transform.getModelMatrix();

    return projectionMatrix * viewMatrix;
}

void SpotLightStorageBuffer::reset() {
    currentSize = 0;
}
