#include "TransformComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../Editor/Systems/EditorUISystem.h"

TransformComponent::TransformComponent() : Component(),
                                           ComponentTransformEdit(),
                                           m_transform(1.0),
                                           m_initialTransform(1.0),
                                           m_isTranslationEnabled(true),
                                           m_isRotationEnabled(true),
                                           m_isScalingEnabled(true),
                                           // m_parentEntityId(0),
                                           // m_parentEntityName(),
                                           // m_shouldUpdateParentEntityId(false),
                                           m_isRelativeRotationDisabled(false),
                                           m_isTransformInSync(false),
                                           m_shouldSyncWorldTransformToLocal(false) {
}

void TransformComponent::decomposeModelMatrix(glm::vec3 &translation, glm::quat &rotation, glm::vec3 &scale) {
    translation = m_transform[3];

    for (int i = 0; i < 3; i++)
        scale[i] = glm::length(glm::vec3(m_transform[i]));

    const glm::mat3 rotMtx(
        glm::vec3(m_transform[0]) / scale[0],
        glm::vec3(m_transform[1]) / scale[1],
        glm::vec3(m_transform[2]) / scale[2]);

    rotation = glm::quat_cast(rotMtx);
}

void TransformComponent::serialize(nlohmann::json &j) {
    serializeLinkedEntity(j);

    glm::vec3 translation = m_initialTransform[3];

    glm::vec3 scale;
    for (int i = 0; i < 3; i++) {
        scale[i] = glm::length(glm::vec3(m_initialTransform[i]));
    }

    const glm::mat3 rotMtx(
        glm::vec3(m_initialTransform[0]) / scale[0],
        glm::vec3(m_initialTransform[1]) / scale[1],
        glm::vec3(m_initialTransform[2]) / scale[2]);

    glm::quat rotation = glm::quat_cast(rotMtx);

    if (m_isTranslationEnabled) {
        j[TRANSLATION_KEY] = translation;
    }
    if (m_isRotationEnabled) {
        j[ROTATION_KEY] = rotation;
    }
    if (m_isScalingEnabled) {
        j[SCALE_KEY] = scale;
    }
    j[REL_ROTATION_KEY] = m_isRelativeRotationDisabled;
}

void TransformComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    deserializeLinkedEntity(j);

    glm::vec3 t = j.value(TRANSLATION_KEY, glm::vec3(0, 0, 0));
    glm::quat r = j.value(ROTATION_KEY, glm::quat(1, 0, 0, 0));
    glm::vec3 s = j.value(SCALE_KEY, glm::vec3(1, 1, 1));

    m_isTranslationEnabled = j.value(ALLOW_TRANSLATION_KEY, m_isTranslationEnabled);
    m_isRotationEnabled = j.value(ALLOW_ROTATION_KEY, m_isRotationEnabled);
    m_isScalingEnabled = j.value(ALLOW_SCALING_KEY, m_isScalingEnabled);
    m_isRelativeRotationDisabled = j.value(REL_ROTATION_KEY, m_isRelativeRotationDisabled);

    if (m_isTranslationEnabled) {
        setTranslation(t);
    }
    if (m_isRotationEnabled) {
        setRotation(r);
    }
    if (m_isScalingEnabled) {
        setScale(s);
    }

    m_initialTransform = m_transform;

    //remove after resave
    std::string name = j.value(PARENT_ENTITY_KEY, "");
    if (!name.empty()) {
        setLinkedEntityName(name);
    }
}

void TransformComponent::setTranslation(glm::vec3 pos) {
    if (m_isTranslationEnabled) {
        m_transform = glm::translate(m_transform, pos);
    }
}

void TransformComponent::setScale(glm::vec3 scale) {
    if (m_isScalingEnabled) {
        m_transform = glm::scale(m_transform, scale);
    }
}

void TransformComponent::setRotation(glm::quat rotation) {
    if (m_isRotationEnabled) {
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        m_transform *= rotationMatrix;
    }
}

void TransformComponent::resetTransform() {
    m_transform = glm::mat4(1.0);
}

glm::mat4 &TransformComponent::getModelMatrix() {
    return m_transform;
}

glm::mat4 TransformComponent::getInverseModelMatrix() const {
    return glm::inverse(m_transform);
}

glm::vec3 TransformComponent::getWorldPosition() {
    return m_transform[3];
}

glm::vec3 TransformComponent::getLocalPosition() {
    return m_initialTransform[3];
}

glm::vec3 TransformComponent::getScale() {
    glm::vec3 scale;
    scale[0] = glm::length(glm::vec3(m_transform[0]));
    scale[1] = glm::length(glm::vec3(m_transform[1]));
    scale[2] = glm::length(glm::vec3(m_transform[2]));

    return scale;
}

glm::quat TransformComponent::getRotation() {
    glm::vec3 scale;
    for (int i = 0; i < 3; i++) {
        scale[i] = glm::length(glm::vec3(m_transform[i]));
    }

    const glm::mat3 rotMtx(
        glm::vec3(m_transform[0]) / scale[0],
        glm::vec3(m_transform[1]) / scale[1],
        glm::vec3(m_transform[2]) / scale[2]);

    return glm::quat_cast(rotMtx);
}

glm::vec3 TransformComponent::getDirection() const {
    glm::quat rotation = glm::quat_cast(m_transform);

    return rotation * glm::vec3(0, 0, 1);
}

glm::vec3 TransformComponent::getInitialTranslation() {
    return m_initialTransform[3];
}

glm::vec3 TransformComponent::getInitialScale() {
    glm::vec3 scale;
    scale[0] = glm::length(glm::vec3(m_initialTransform[0]));
    scale[1] = glm::length(glm::vec3(m_initialTransform[1]));
    scale[2] = glm::length(glm::vec3(m_initialTransform[2]));

    return scale;
}

glm::quat TransformComponent::getInitialRotation() {
    glm::vec3 scale = getInitialScale();
    const glm::mat3 rotMtx(
        glm::vec3(m_initialTransform[0]) / scale[0],
        glm::vec3(m_initialTransform[1]) / scale[1],
        glm::vec3(m_initialTransform[2]) / scale[2]);

    return glm::quat_cast(rotMtx);
}

glm::mat4 TransformComponent::getWorldTransform() {
    return m_transform;
}

void TransformComponent::setWorldTransform(const glm::mat4 &m) {
    m_transform = m;

    if (isLinkableToEntity() > 0) {
        m_shouldSyncWorldTransformToLocal = true;
    } else {
        m_initialTransform = m;
    }
}

void TransformComponent::updateTransformFromParent(const glm::mat4 &parentWorldSpaceTransform) {
    if (m_shouldSyncWorldTransformToLocal) {
        m_initialTransform = glm::inverse(parentWorldSpaceTransform) * m_transform;
        m_shouldSyncWorldTransformToLocal = false;
    } else {
        m_transform = parentWorldSpaceTransform * m_initialTransform;
    }

    m_isTransformInSync = true;
}

void TransformComponent::updateTransformWorld() {
    m_isTransformInSync = true;
}

void TransformComponent::setWorldRotation(const glm::mat4 &rotationMatrix) {
    if (!m_isTransformInSync) {
        return;
    }

    const auto translation = glm::vec3(m_transform[3]);

    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(m_transform[0]));
    scale.y = glm::length(glm::vec3(m_transform[1]));
    scale.z = glm::length(glm::vec3(m_transform[2]));

    auto rotation = glm::mat3(rotationMatrix);

    auto result = glm::mat4(1.0f);
    result[0] = glm::vec4(glm::normalize(rotation[0]) * scale.x, 0.0f);
    result[1] = glm::vec4(glm::normalize(rotation[1]) * scale.y, 0.0f);
    result[2] = glm::vec4(glm::normalize(rotation[2]) * scale.z, 0.0f);
    result[3] = glm::vec4(translation, 1.0f);

    setWorldTransform(result);
}

void TransformComponent::setLocalRotation(const glm::mat4 &rotationMatrix) {
    const auto translation = glm::vec3(m_initialTransform[3]);

    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(m_initialTransform[0]));
    scale.y = glm::length(glm::vec3(m_initialTransform[1]));
    scale.z = glm::length(glm::vec3(m_initialTransform[2]));

    auto rotation = glm::mat3(rotationMatrix);

    m_initialTransform = glm::mat4(1.0f);
    m_initialTransform[0] = glm::vec4(glm::normalize(rotation[0]) * scale.x, 0.0f);
    m_initialTransform[1] = glm::vec4(glm::normalize(rotation[1]) * scale.y, 0.0f);
    m_initialTransform[2] = glm::vec4(glm::normalize(rotation[2]) * scale.z, 0.0f);
    m_initialTransform[3] = glm::vec4(translation, 1.0f);
}
