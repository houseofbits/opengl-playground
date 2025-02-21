#include "PhysicsHingeJointComponentEdit.h"

#include "../../../Core/Helper/Math.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

void PhysicsHingeJointComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    if (EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    )) {
        m_linkedTransformA = getTransformComponentByName(m_component->m_targetEntityAName);
    }

    if (EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    )) {
        m_linkedTransformB = getTransformComponentByName(m_component->m_targetEntityBName);
    }
    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(m_component->m_localAttachmentMatrixA, true, true, false);

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(m_component->m_localAttachmentMatrixB, true, true, false);

        ImGui::TreePop();
    }

    ImGui::Checkbox("Angular limits", &m_component->m_isLimitsEnabled);
    ImGui::Checkbox("Lock on limits", &m_component->m_isLockingToLimitsEnabled);

    if (m_component->m_isLimitsEnabled) {
        if (ImGui::InputFloat2("Angular limits", reinterpret_cast<float *>(&angularLimits))) {
            m_component->m_angularLimits.x = glm::radians(angularLimits.x);
            m_component->m_angularLimits.y = glm::radians(angularLimits.y);
        } else {
            angularLimits.x = glm::degrees(m_component->m_angularLimits.x);
            angularLimits.y = glm::degrees(m_component->m_angularLimits.y);
        }
    }
}

PhysicsHingeJointComponentEdit::TransformOption PhysicsHingeJointComponentEdit::getTransformTargetOptions(int index) {
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

glm::mat4 PhysicsHingeJointComponentEdit::getWorldSpaceTransform(int index) {
    if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
        return {};
    }

    if (index == 2) {
        auto m = m_component->m_localAttachmentMatrixB;
        const glm::mat4 transformB = Math::rescaleMatrix(m_linkedTransformB->m_transform);

        return transformB * m;
    }

    auto m = m_component->m_localAttachmentMatrixA;
    const glm::mat4 transformA = Math::rescaleMatrix(m_linkedTransformA->m_transform);

    return transformA * m;
}

void PhysicsHingeJointComponentEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
    if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
        return;
    }

    const glm::mat4 transformA = glm::inverse(Math::rescaleMatrix(m_linkedTransformA->m_transform));
    if (index == 1) {
        m_component->m_localAttachmentMatrixA = transformA * m;

        return;
    }

    const glm::mat4 transformB = glm::inverse(Math::rescaleMatrix(m_linkedTransformB->m_transform));
    if (index == 2) {
        m_component->m_localAttachmentMatrixB = transformB * m;

        return;
    }

    m_component->m_localAttachmentMatrixA = transformA * m;;
    m_component->m_localAttachmentMatrixB = transformB * m;
}

void PhysicsHingeJointComponentEdit::handleEntitySelection(Entity &e, Component *c) {
    ComponentEdit::handleEntitySelection(e, c);

    m_linkedTransformA = getTransformComponentByName(m_component->m_targetEntityAName);
    m_linkedTransformB = getTransformComponentByName(m_component->m_targetEntityBName);
}

TransformComponent *PhysicsHingeJointComponentEdit::getTransformComponentByName(const std::string &name) const {
    if (name.empty()) {
        return m_entity->getComponent<TransformComponent>();
    }

    return m_editorSystem->m_EntityContext->findEntityComponent<TransformComponent>(name);
}
