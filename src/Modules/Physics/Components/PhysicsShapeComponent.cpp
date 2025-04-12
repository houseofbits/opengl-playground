#include "PhysicsShapeComponent.h"
#include "../../../Core/Helper/Math.h"
#include <glm/gtx/quaternion.hpp>

PhysicsShapeComponent::PhysicsShapeComponent() : Component() {
}

void PhysicsShapeComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = m_type;
    j[POS_KEY] = m_localPosition;
    j[ROT_KEY] = m_localRotation;
    if (m_type == TYPE_BOX) {
        j[BOX_SIZE_KEY] = m_boxSize;
    }
    if (m_type == TYPE_SPHERE) {
        j[SPHERE_RADIUS_KEY] = m_sphereRadius;
    }
    if (m_type == TYPE_MESH) {
        j[MESH_FILE_KEY] = m_meshResource().m_Path;
    }
}

void PhysicsShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_type = j.value(TYPE_KEY, m_type);
    m_localPosition = j.value(POS_KEY, m_localPosition);
    m_localRotation = j.value(ROT_KEY, m_localRotation);

    if (m_type == TYPE_MESH) {
        std::string path = j.value(MESH_FILE_KEY, m_meshResource().m_Path);
        resourceManager.request(m_meshResource, path);
    }
    if (m_type == TYPE_BOX) {
        m_boxSize = j.value(BOX_SIZE_KEY, m_boxSize);
    }
    if (m_type == TYPE_SPHERE) {
        m_sphereRadius = j.value(SPHERE_RADIUS_KEY, m_sphereRadius);
    }
}

glm::mat4 PhysicsShapeComponent::getWorldTransform(const glm::mat4 &parentTransform) const {
    glm::mat4 localTransform = glm::mat4(1.0f);
    localTransform = glm::toMat4(m_localRotation);

    glm::vec3 scale = glm::vec3(1.0);
    if (m_type == TYPE_BOX) {
        scale = m_boxSize;
    }
    if (m_type == TYPE_SPHERE) {
        scale = glm::vec3(m_sphereRadius);
    }

    localTransform = glm::scale(localTransform, scale);
    localTransform[3] = glm::vec4(m_localPosition, 1.0);

    return Math::rescaleMatrix(parentTransform) * localTransform;
}
