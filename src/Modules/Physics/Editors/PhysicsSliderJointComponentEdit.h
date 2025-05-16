#pragma once

#include "StatefulJointBehaviourEdit.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../Components/PhysicsDistanceJointComponent.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processPhysicsSliderJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsSliderJointComponent *>(c);

    ImGui::Text("Slide on X axis");

    ImGui::Checkbox("Is initially connected", &component->m_isInitiallyConnected);

    // EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
    //     *system.m_EntityContext,
    //     component->m_targetEntityAName,
    //     "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
    //     "Self"
    // );

    EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
    *system.m_EntityContext,
    component->m_targetEntityName,
        "Attachment entity##TRANSFORM_PARENT_ENTITY_NAME_B"
    );

    // ImGui::InputFloat3("Axis", reinterpret_cast<float *>(&component->m_axis));
    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, true, false);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, true, false);
        ImGui::TreePop();
    }

    ImGui::Checkbox("Limits", &component->m_isLimitsEnabled);

    if (component->m_isLimitsEnabled) {
        ImGui::InputFloat2("Min/Max", reinterpret_cast<float *>(&component->m_limits));
    }

    ImGui::Checkbox("Motor", &component->m_isMotorSettingsEnabled);

    if (component->m_isMotorSettingsEnabled) {
        ImGui::DragFloat("Force limit", &component->m_motorForceLimit, 0.1f);
        ImGui::DragFloat("Damping", &component->m_motorDamping, 0.1f);
        ImGui::DragFloat("Frequency", &component->m_motorFrequency, 0.1f);
    }
}