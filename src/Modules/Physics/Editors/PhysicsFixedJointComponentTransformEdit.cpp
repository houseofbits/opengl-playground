#include "PhysicsFixedJointComponentTransformEdit.h"
#include "../../../Core/Helper/Math.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/PhysicsFixedJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

PhysicsFixedJointComponentTransformEdit::TransformOption PhysicsFixedJointComponentTransformEdit::getTransformTargetOptions(int index) {
    switch (index) {
        case 1:
            return {
                "Attachment point A",
                true,
                false,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
        case 2:
            return {
                "Attachment point B",
                true,
                false,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
        default:
            return {
                "Common attachment point",
                true,
                false,
                false,
                ImGuizmo::TRANSLATE,
                false,
                true,
            };
    };
}

glm::mat4 PhysicsFixedJointComponentTransformEdit::getWorldSpaceTransform(int index) {
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

void PhysicsFixedJointComponentTransformEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
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

void PhysicsFixedJointComponentTransformEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_linkedTransformA = e.getComponent<TransformComponent>();
    m_linkedTransformB = getTransformComponentByName(m_component->m_targetEntityName);
}

TransformComponent *PhysicsFixedJointComponentTransformEdit::getTransformComponentByName(const std::string &name) const {
    if (name.empty()) {
        return m_entity->getComponent<TransformComponent>();
    }

    return m_editorSystem->m_EntityContext->findEntityComponent<TransformComponent>(name);
}
