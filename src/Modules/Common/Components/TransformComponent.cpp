#include "TransformComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../../Physics/Systems/PhysicsSystem.h"
#include "../../Renderer/Systems/EnvironmentProbeRenderSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../../Renderer/Systems/ShadowMapRenderSystem.h"
#include "../../Renderer/Systems/StorageBufferUpdateSystem.h"

TransformComponent::TransformComponent() : m_transform(1.0),
                                           m_isTranslationEnabled(true),
                                           m_isRotationEnabled(true),
                                           m_isScalingEnabled(true){
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<StorageBufferUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<ShadowMapRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
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
    glm::vec3 t;
    glm::quat r;
    glm::vec3 s;

    decomposeModelMatrix(t, r, s);

    if (m_isTranslationEnabled) {
        j[TRANSLATION_KEY] = t;
    }
    if (m_isRotationEnabled) {
        j[ROTATION_KEY] = r;
    }
    if (m_isScalingEnabled) {
        j[SCALE_KEY] = s;
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

glm::vec3 TransformComponent::getDirection() const {
    glm::quat rotation = glm::quat_cast(m_transform);

    return rotation * glm::vec3(0, 0, 1);
}

physx::PxTransform TransformComponent::getPxTransform() {
    glm::quat rq = glm::quat_cast(m_transform);

    physx::PxQuat rotation(rq.x, rq.y, rq.z, rq.w);

    physx::PxTransform transform(m_transform[3].x, m_transform[3].y, m_transform[3].z, rotation);

    return transform;
}

void TransformComponent::setFromPxTransform(physx::PxTransform transform) {
    auto rotation = physx::PxMat33(transform.q);

    m_transform[0][0] = rotation.column0[0];
    m_transform[0][1] = rotation.column0[1];
    m_transform[0][2] = rotation.column0[2];
    m_transform[0][3] = 0;

    m_transform[1][0] = rotation.column1[0];
    m_transform[1][1] = rotation.column1[1];
    m_transform[1][2] = rotation.column1[2];
    m_transform[1][3] = 0;

    m_transform[2][0] = rotation.column2[0];
    m_transform[2][1] = rotation.column2[1];
    m_transform[2][2] = rotation.column2[2];
    m_transform[2][3] = 0;

    m_transform[3][0] = transform.p[0];
    m_transform[3][1] = transform.p[1];
    m_transform[3][2] = transform.p[2];
    m_transform[3][3] = 1;
}
