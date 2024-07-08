#include "TransformComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../../Renderer/Systems/EnvironmentProbeRenderSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../../Renderer/Systems/StorageBufferUpdateSystem.h"

TransformComponent::TransformComponent() : m_ModelMatrix(1.0) {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<StorageBufferUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void TransformComponent::decomposeModelMatrix(glm::vec3 &translation, glm::quat &rotation, glm::vec3 &scale) {
    translation = m_ModelMatrix[3];

    for (int i = 0; i < 3; i++)
        scale[i] = glm::length(glm::vec3(m_ModelMatrix[i]));

    const glm::mat3 rotMtx(
            glm::vec3(m_ModelMatrix[0]) / scale[0],
            glm::vec3(m_ModelMatrix[1]) / scale[1],
            glm::vec3(m_ModelMatrix[2]) / scale[2]);

    rotation = glm::quat_cast(rotMtx);
}
void TransformComponent::serialize(nlohmann::json &j) {
    glm::vec3 t;
    glm::quat r;
    glm::vec3 s;

    decomposeModelMatrix(t, r, s);

    if (m_isTranslationEnabled) {
        j["translation"] = t;
    }
    if (m_isRotationEnabled) {
        j["rotation"] = r;
    }
    if (m_isScalingEnabled) {
        j["scale"] = s;
    }
}

void TransformComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    glm::vec3 t = j.value("translation", glm::vec3(0, 0, 0));
    glm::quat r = j.value("rotation", glm::quat(1, 0, 0, 0));
    glm::vec3 s = j.value("scale", glm::vec3(1, 1, 1));

    m_isTranslationEnabled = j.value("allowTranslation", m_isTranslationEnabled);
    m_isRotationEnabled = j.value("allowRotation", m_isRotationEnabled);
    m_isScalingEnabled = j.value("allowScaling", m_isScalingEnabled);

    if (m_isTranslationEnabled) {
        setTranslation(t);
    }
    if (m_isRotationEnabled) {
        setRotation(r);
    }
    if (m_isScalingEnabled) {
        setScale(s);
    }
}

void TransformComponent::setTranslation(glm::vec3 pos) {
    if (m_isTranslationEnabled) {
        m_ModelMatrix = glm::translate(m_ModelMatrix, pos);
    }
}

void TransformComponent::setScale(glm::vec3 scale) {
    if (m_isScalingEnabled) {
        m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
    }
}

void TransformComponent::setRotation(glm::quat rotation) {
    if (m_isRotationEnabled) {
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        m_ModelMatrix *= rotationMatrix;
    }
}

void TransformComponent::resetTransform() {
    m_ModelMatrix = glm::mat4(1.0);
}

glm::mat4 &TransformComponent::getModelMatrix() {
    return m_ModelMatrix;
}

glm::mat4 TransformComponent::getInverseModelMatrix() const {
    return glm::inverse(m_ModelMatrix);
}

glm::vec3 TransformComponent::getTranslation() {
    return m_ModelMatrix[3];
}

glm::vec3 TransformComponent::getScale() {
    glm::vec3 scale;
    scale[0] = glm::length(glm::vec3(m_ModelMatrix[0]));
    scale[1] = glm::length(glm::vec3(m_ModelMatrix[1]));
    scale[2] = glm::length(glm::vec3(m_ModelMatrix[2]));

    return scale;
}

glm::vec3 TransformComponent::getDirection() const {
    glm::quat rotation = glm::quat_cast(m_ModelMatrix);

    return rotation * glm::vec3(0, 0, 1);
}
