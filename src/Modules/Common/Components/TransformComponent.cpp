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

TransformComponent::TransformComponent() : m_transform(1.0),
                                           m_initialTransform(1.0),
                                           m_isTranslationEnabled(true),
                                           m_isRotationEnabled(true),
                                           m_isScalingEnabled(true) {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystemHaving<MainRenderSystem, StaticMeshComponent>(this);
    ctx.registerComponentWithEntitySystemHaving<EnvironmentProbeRenderSystem, EnvironmentProbeComponent>(this);
    ctx.registerComponentWithEntitySystem<StorageBufferUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<ShadowMapRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);             //Multiple dependencies
    ctx.registerComponentWithEntitySystemHaving<PhysicsBodyProcessingSystem, PhysicsBodyComponent>(this);
    ctx.registerComponentWithEntitySystemHaving<CharacterControllerSystem, CharacterControllerComponent>(this);

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

//void TransformComponent::setFromPxTransform(const physx::PxTransform &transform) {
//    glm::vec3 scale = getScale();
//    m_transform = glm::mat4(1.0);
//    setTranslation(glm::vec3(transform.p.x, transform.p.y, transform.p.z));
//    setRotation(glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z));
//    setScale(scale);

    //    auto rotation = physx::PxMat33(transform.q);
    //    m_transform[0][0] = rotation.column0[0];
    //    m_transform[0][1] = rotation.column0[1];
    //    m_transform[0][2] = rotation.column0[2];
    //    m_transform[0][3] = 0;
    //
    //    m_transform[1][0] = rotation.column1[0];
    //    m_transform[1][1] = rotation.column1[1];
    //    m_transform[1][2] = rotation.column1[2];
    //    m_transform[1][3] = 0;
    //
    //    m_transform[2][0] = rotation.column2[0];
    //    m_transform[2][1] = rotation.column2[1];
    //    m_transform[2][2] = rotation.column2[2];
    //    m_transform[2][3] = 0;
    //
    //    m_transform[3][0] = transform.p[0];
    //    m_transform[3][1] = transform.p[1];
    //    m_transform[3][2] = transform.p[2];
    //    m_transform[3][3] = 1;
//}

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
