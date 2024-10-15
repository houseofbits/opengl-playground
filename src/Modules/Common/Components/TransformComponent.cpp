#include "TransformComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../Physics/Systems/CharacterControllerSystem.h"
#include "../../Physics/Systems/PhysicsBodyProcessingSystem.h"
#include "../../Physics/Systems/PhysicsSystem.h"
#include "../../Renderer/Systems/EnvironmentProbeRenderSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../../Renderer/Systems/ShadowMapRenderSystem.h"
#include "../../Renderer/Systems/StorageBufferUpdateSystem.h"
#include "../Systems/TransformHierarchyProcessingSystem.h"

TransformComponent::TransformComponent() : Component(),
                                           ComponentTransformEdit(),
                                           m_transform(1.0),
                                           m_initialTransform(1.0),
                                           m_isTranslationEnabled(true),
                                           m_isRotationEnabled(true),
                                           m_isScalingEnabled(true),
                                           m_parentEntityId(0),
                                           m_parentEntityName(),
                                           m_shouldUpdateParentEntityId(false),
                                           m_shouldSyncWorldTransformToLocal(false) {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<TransformHierarchyProcessingSystem>(this);
    ctx.registerComponentWithEntitySystemHaving<MainRenderSystem, StaticMeshComponent>(this);
    ctx.registerComponentWithEntitySystemHaving<EnvironmentProbeRenderSystem, EnvironmentProbeComponent>(this);
    ctx.registerComponentWithEntitySystem<StorageBufferUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<ShadowMapRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);             //Multiple dependencies
    ctx.registerComponentWithEntitySystemHaving<PhysicsBodyProcessingSystem, PhysicsBodyComponent>(this);
    ctx.registerComponentWithEntitySystemHaving<CharacterControllerSystem, PhysicsCharacterComponent>(this);

    //    ctx.getEntitySystem<DoorUpdateSystem>().entityHaving<DoorComponent>().registerComponent(this); ????
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
    if (m_parentEntityId > 0) {
        j[PARENT_ENTITY_KEY] = m_parentEntityName;
    }
}

void TransformComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    glm::vec3 t = j.value(TRANSLATION_KEY, glm::vec3(0, 0, 0));
    glm::quat r = j.value(ROTATION_KEY, glm::quat(1, 0, 0, 0));
    glm::vec3 s = j.value(SCALE_KEY, glm::vec3(1, 1, 1));

    m_isTranslationEnabled = j.value(ALLOW_TRANSLATION_KEY, m_isTranslationEnabled);
    m_isRotationEnabled = j.value(ALLOW_ROTATION_KEY, m_isRotationEnabled);
    m_isScalingEnabled = j.value(ALLOW_SCALING_KEY, m_isScalingEnabled);

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

    m_parentEntityName = j.value(PARENT_ENTITY_KEY, m_parentEntityName);
    if (!m_parentEntityName.empty()) {
        m_shouldUpdateParentEntityId = true;
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

glm::vec3 TransformComponent::getTranslation() {
    return m_transform[3];
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

glm::mat4 TransformComponent::getEditorTransform() {
    return m_transform;
}

void TransformComponent::setFromEditorTransform(const glm::mat4 &m) {
    m_transform = m;

    if (m_parentEntityId > 0) {
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
}
