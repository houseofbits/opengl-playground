#include "PhysicsSliderJointComponentTransformEdit.h"
#include "PhysicsSliderJointComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../../Core/Helper/Math.h"

void PhysicsSliderJointComponentTransformEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_linkedTransformA = getTransformComponentByName(m_component->m_targetEntityAName);
    m_linkedTransformB = getTransformComponentByName(m_component->m_targetEntityBName);
}

TransformComponent *
PhysicsSliderJointComponentTransformEdit::getTransformComponentByName(const std::string &name) const {
    if (name.empty()) {
        return m_entity->getComponent<TransformComponent>();
    }

    return m_editorSystem->m_EntityContext->findEntityComponent<TransformComponent>(name);
}

PhysicsSliderJointComponentTransformEdit::TransformOption
PhysicsSliderJointComponentTransformEdit::getTransformTargetOptions(int index) {
    switch (index) {
        case 1:
            return {
                "Attachment point A",
                true,
                true,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
        case 2:
            return {
                "Attachment point B",
                true,
                true,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
        default:
            return {
                "Common attachment point",
                true,
                true,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
    };
}

glm::mat4 PhysicsSliderJointComponentTransformEdit::getWorldSpaceTransform(int index) {
    if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
        return {};
    }

    if (index == 2) {
        auto m = m_component->m_localAttachmentMatrixB;
        const glm::mat4 transformB = Math::rescaleMatrix(m_linkedTransformB->getWorldTransform());

        return transformB * m;
    }

    auto m = m_component->m_localAttachmentMatrixA;
    const glm::mat4 transformA = Math::rescaleMatrix(m_linkedTransformA->getWorldTransform());

    return transformA * m;
}

void PhysicsSliderJointComponentTransformEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
    if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
        return;
    }

    const glm::mat4 transformA = glm::inverse(Math::rescaleMatrix(m_linkedTransformA->getWorldTransform()));
    if (index == 1) {
        m_component->m_localAttachmentMatrixA = transformA * m;

        return;
    }

    const glm::mat4 transformB = glm::inverse(Math::rescaleMatrix(m_linkedTransformB->getWorldTransform()));
    if (index == 2) {
        m_component->m_localAttachmentMatrixB = transformB * m;

        return;
    }

    m_component->m_localAttachmentMatrixA = transformA * m;;
    m_component->m_localAttachmentMatrixB = transformB * m;
}
