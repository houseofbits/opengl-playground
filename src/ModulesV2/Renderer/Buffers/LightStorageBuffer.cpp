#include "LightStorageBuffer.h"
#include "../../../Helper/ShaderSourceLoader.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

template class ShaderStorageBuffer<SpotLightStructure>;

const glm::vec3 CUBE_DIRECTIONS[] = {
        glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, -1),
};

LightStorageBuffer::LightStorageBuffer() : ShaderStorageBuffer<SpotLightStructure>(0) {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("SpotLightStorageBuffer");
}

void LightStorageBuffer::initialize() {
    create(MAX_SPOT_LIGHTS, uniformBindingIndex);
}

void LightStorageBuffer::appendLight(TransformComponent &transform, LightComponent &light, int projectorIndex) {
    if (light.m_Type == LightComponent::SPOT) {
        SpotLightStructure structure;
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
        SpotLightStructure structure;
        structure.projectionViewMatrix = createOrthoProjectionViewMatrix(transform, light);
        structure.color = light.m_Color;
        structure.intensity = light.m_Intensity;
        structure.position = transform.getTranslation();
        structure.direction = transform.getDirection();
        structure.attenuation = light.m_Attenuation;
        structure.projectorSamplerIndex = projectorIndex;
        structure.isPointSource = 0;

        append(structure);
    } else if (light.m_Type == LightComponent::OMNI) {
        for (auto side : CUBE_DIRECTIONS) {
            SpotLightStructure structure;

            structure.projectionViewMatrix = createPerspectiveProjectionViewMatrix(side, transform.getTranslation(), light.m_Attenuation);
            structure.color = light.m_Color;
            structure.intensity = light.m_Intensity;
            structure.position = transform.getTranslation();
            structure.direction = transform.getDirection();
            structure.attenuation = light.m_Attenuation;
            structure.projectorSamplerIndex = projectorIndex;
            structure.isPointSource = 1;

            append(structure);
        }
    }
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

glm::mat4 LightStorageBuffer::createPerspectiveProjectionViewMatrix(glm::vec3 direction, glm::vec3 position, float far) {
    glm::vec3 dir = glm::normalize(direction);
    glm::vec3 up(0, 1, 0);
    if (fabs(glm::dot(dir, up)) > 0.99) {
        up = glm::vec3(1, 0, 0);
    }

    glm::vec3 cross = glm::cross(dir, up);
    glm::vec3 tangentUp = glm::cross(dir, cross);

    glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(90.0), 1.0, 0.01, far);
    glm::mat4 viewMatrix = glm::lookAt(position, position + direction, tangentUp);
    return projectionMatrix * viewMatrix;
}
