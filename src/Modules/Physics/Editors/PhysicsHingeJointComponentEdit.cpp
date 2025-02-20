#include "PhysicsHingeJointComponentEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

void PhysicsHingeJointComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    );

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    );

    ImGui::InputFloat3("Attachment A", reinterpret_cast<float *>(&m_component->m_localAttachmentA));
    ImGui::InputFloat3("Axis A", reinterpret_cast<float *>(&m_component->m_axisA));
    ImGui::InputFloat3("Attachment B", reinterpret_cast<float *>(&m_component->m_localAttachmentB));
    ImGui::InputFloat3("Axis B", reinterpret_cast<float *>(&m_component->m_axisB));

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
    return {
        "Common attachment point",
        true,
        true,
        false,
        ImGuizmo::TRANSLATE,
        false,
        true,
    };
}

glm::mat4 PhysicsHingeJointComponentEdit::getWorldSpaceTransform(int index) {
    return {};
}

void PhysicsHingeJointComponentEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
}
