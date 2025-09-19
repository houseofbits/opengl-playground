#include "PhysicsShapeComponentTransformEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../../Core/Helper/Math.h"

void PhysicsShapeComponentTransformEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_parentTransform = e.getComponent<TransformComponent>();
}

PhysicsShapeComponentTransformEdit::TransformOption
PhysicsShapeComponentTransformEdit::getTransformTargetOptions(int index) {
    return {
        "Attachment point A",
        true,
        true,
        true,
        ImGuizmo::TRANSLATE,
        false,
        true,
    };
}

glm::mat4 PhysicsShapeComponentTransformEdit::getWorldSpaceTransform(int index) {
    glm::mat4 parentTransform = glm::mat4(1.0);

    if (m_parentTransform != nullptr) {
        parentTransform = Math::rescaleMatrix(m_parentTransform->getWorldTransform());
    }

    return m_component->getWorldTransform(parentTransform);
}

void PhysicsShapeComponentTransformEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
    auto transform = m;
    auto scaling = Math::getScale(transform);
    if (m_parentTransform != nullptr) {
        transform = glm::inverse(Math::rescaleMatrix(m_parentTransform->getWorldTransform())) * m;
    }

    m_component->m_localRotation = Math::getRotation(transform);
    m_component->m_localPosition = transform[3];

    if (m_component->m_type == PhysicsShapeComponent::TYPE_BOX) {
        m_component->m_boxSize = scaling;
    }

    if (m_component->m_type == PhysicsShapeComponent::TYPE_SPHERE) {
        m_component->m_sphereRadius = scaling.x;
    }

    if (m_component->m_type == PhysicsShapeComponent::TYPE_MESH) {
        m_component->m_meshScale = scaling;
    }
}
