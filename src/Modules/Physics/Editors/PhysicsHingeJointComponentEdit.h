#pragma once

#include "StatefulJointBehaviourEdit.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../../Core/Helper/Math.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processPhysicsHingeJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsHingeJointComponent *>(c);

    ImGui::Checkbox("Is initially connected", &component->m_isInitiallyConnected);

    ImGui::Text("Rotate around X axis");

    // EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
    //     *system.m_EntityContext,
    //     component->m_targetEntityAName,
    //     "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
    //     "Self"
    // );

    EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
        *system.m_EntityContext,
        component->m_targetEntityName,
        "Attachment entity##TRANSFORM_PARENT_ENTITY_NAME"
    );

    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, true, false);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, true, false);
        ImGui::TreePop();
    }

    ImGui::Checkbox("Angular limits", &component->m_isLimitsEnabled);
    ImGui::Checkbox("Lock on limits", &component->m_isLockingToLimitsEnabled);

    static glm::vec2 angularLimits{0, 0};
    if (component->m_isLimitsEnabled) {
        if (ImGui::InputFloat2("Angular limits", reinterpret_cast<float *>(&angularLimits))) {
            component->m_angularLimits.x = glm::radians(angularLimits.x);
            component->m_angularLimits.y = glm::radians(angularLimits.y);
        } else {
            angularLimits.x = glm::degrees(component->m_angularLimits.x);
            angularLimits.y = glm::degrees(component->m_angularLimits.y);
        }
    }

    ImGui::Checkbox("Motor settings", &component->m_isMotorSettingsEnabled);
    if (component->m_isMotorSettingsEnabled) {
        ImGui::InputFloat("Force limit", &component->m_motorForceLimit);
        ImGui::InputFloat("Torque limit", &component->m_motorTorqueLimit);
        ImGui::InputFloat("Damping", &component->m_motorDamping);
        ImGui::InputFloat("Frequency", &component->m_motorFrequency);
    }

    if (ImGui::TreeNode("Stateful behaviour")) {
        ImGui::Checkbox("Enabled", &component->m_useStatefulJointBehaviour);

        StatefulJointBehaviourEditor(&component->m_statefulJointBehaviour, system);

        ImGui::TreePop();
    }
}
