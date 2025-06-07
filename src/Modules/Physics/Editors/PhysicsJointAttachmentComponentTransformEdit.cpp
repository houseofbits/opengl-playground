#include "PhysicsJointAttachmentComponentTransformEdit.h"
#include "PhysicsSliderJointComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../Components//PhysicsCharacterComponent.h"
#include "../../../Core/Helper/Math.h"

void PhysicsJointAttachmentComponentTransformEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_parentTransform = e.getComponent<TransformComponent>();
    m_parentCharacter = e.getComponent<PhysicsCharacterComponent>();
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
    glm::mat4 transform = Math::rescaleMatrix(m_parentTransform->getWorldTransform());

    if (m_parentCharacter) {
        auto py = m_parentCharacter->m_stepTolerance + (
                      (m_parentCharacter->m_height - m_parentCharacter->m_stepTolerance) * 0.5);

        transform = glm::translate(transform, glm::vec3(0,  py, 0));
    }

    return transform * m;
}

void PhysicsJointAttachmentComponentTransformEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
    if (m_parentTransform == nullptr) {
        return;
    }
    auto transform = Math::rescaleMatrix(m_parentTransform->getWorldTransform());

    if (m_parentCharacter) {
        auto py = m_parentCharacter->m_stepTolerance + (
                      (m_parentCharacter->m_height - m_parentCharacter->m_stepTolerance) * 0.5);

        transform = glm::translate(transform, glm::vec3(0, py, 0));
    }

    m_component->m_localAttachmentMatrix = glm::inverse(transform) * m;
}
