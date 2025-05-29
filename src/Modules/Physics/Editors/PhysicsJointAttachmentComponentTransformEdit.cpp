#include "PhysicsJointAttachmentComponentTransformEdit.h"
#include "PhysicsSliderJointComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../../Core/Helper/Math.h"

void PhysicsJointAttachmentComponentTransformEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_parentTransform = e.getComponent<TransformComponent>();
}

PhysicsJointAttachmentComponentTransformEdit::TransformOption
PhysicsJointAttachmentComponentTransformEdit::getTransformTargetOptions(int index) {
    return {
        "Attachment point",
        true,
        true,
        false,
        ImGuizmo::TRANSLATE,
        false,
        true,
    };
}

glm::mat4 PhysicsJointAttachmentComponentTransformEdit::getWorldSpaceTransform(int index) {
    if (m_parentTransform == nullptr) {
        return {};
    }

    auto m = m_component->m_localAttachmentMatrix;
    const glm::mat4 transform = Math::rescaleMatrix(m_parentTransform->getWorldTransform());

    return transform * m;
}

void PhysicsJointAttachmentComponentTransformEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
    if (m_parentTransform == nullptr) {
        return;
    }

    const glm::mat4 transform = glm::inverse(Math::rescaleMatrix(m_parentTransform->getWorldTransform()));
    m_component->m_localAttachmentMatrix = transform * m;
}
