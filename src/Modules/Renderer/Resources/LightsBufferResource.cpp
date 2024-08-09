#include "LightsBufferResource.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"

const glm::vec3 CUBE_DIRECTIONS[] = {
        glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, -1),
};

LightsBufferResource::LightsBufferResource() : Resource(), m_StorageBuffer() {
}

Resource::Status LightsBufferResource::build() {
    if (!m_StorageBuffer.isSupported()) {
        return STATUS_BUILD_ERROR;
    }

    m_StorageBuffer.create(
            MAX_SPOT_LIGHTS,
            ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void LightsBufferResource::destroy() {
}

void LightsBufferResource::appendLight(TransformComponent &transform, LightComponent &light) {

    light.m_lightBufferIndices.clear();
    for (int i = 0; i < light.getNumberOfBufferLights(); i++) {
        LightStructure structure;
        structure.color = light.m_Color;
        structure.intensity = light.m_Intensity;
        structure.position = transform.getTranslation();
        structure.direction = transform.getDirection();
        structure.attenuation = light.m_Attenuation;
        structure.projectorSamplerHandle = 0;
        structure.shadowSamplerHandle = 0;
        structure.isPointSource = light.m_Type == LightComponent::DIRECT ? 0 : 1;
        structure.bias = light.m_shadowBias;

        if (light.m_Projection().isReady()) {
            structure.projectorSamplerHandle = light.m_Projection().m_handleId;
        }

        if (light.m_doesCastShadows && light.m_ShadowMaps.size() > i && light.m_ShadowMaps[i]->get().isReady()) {
            structure.shadowSamplerHandle = light.m_ShadowMaps[i]->get().m_handleId;
        }

        switch (light.m_Type) {
            case LightComponent::OMNI:
                structure.projectionViewMatrix = createPerspectiveProjectionViewMatrix(CUBE_DIRECTIONS[i], transform.getTranslation(), light.m_Attenuation);
                break;
            case LightComponent::SPOT:
                structure.projectionViewMatrix = createPerspectiveProjectionViewMatrix(transform, light);
                break;
            case LightComponent::DIRECT:
                structure.projectionViewMatrix = createOrthoProjectionViewMatrix(transform, light);
                break;
        }

        m_StorageBuffer.append(structure);
        light.m_lightBufferIndices.push_back((int) m_StorageBuffer.currentSize - 1);
    }
}

void LightsBufferResource::bind(ShaderProgramResource &shader) {
    m_StorageBuffer.bind();
    shader.setUniform(getSizeAttributeName().c_str(), m_StorageBuffer.currentSize);
}

glm::mat4 LightsBufferResource::createPerspectiveProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {

    glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(light.m_beamAngle), 1.0, 0.01, light.m_Attenuation);

    return projectionMatrix * transform.getInverseModelMatrix();
}

glm::mat4 LightsBufferResource::createOrthoProjectionViewMatrix(TransformComponent &transform, LightComponent &light) {
    glm::mat4 projectionMatrix = glm::ortho<float>(-light.m_Radius, light.m_Radius, -light.m_Radius, light.m_Radius, 0.01, light.m_Attenuation);

    return projectionMatrix * transform.getInverseModelMatrix();
}

glm::mat4 LightsBufferResource::createPerspectiveProjectionViewMatrix(glm::vec3 direction, glm::vec3 position, float far) {
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
