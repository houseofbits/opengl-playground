#pragma once

#include "StatefulJointBehaviourEdit.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../../Core/Helper/Math.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../Components/PhysicsSwingTwistJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processPhysicsSwingTwistJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsSwingTwistJointComponent *>(c);

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

    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, true, false);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, true, false);
        ImGui::TreePop();
    }

    static glm::vec2 angularLimits{0, 0};
    if (ImGui::InputFloat2("Twist limits (Y)", reinterpret_cast<float *>(&angularLimits))) {
        component->m_twistAngleLimits.x = glm::radians(angularLimits.x);
        component->m_twistAngleLimits.y = glm::radians(angularLimits.y);
    } else {
        angularLimits.x = glm::degrees(component->m_twistAngleLimits.x);
        angularLimits.y = glm::degrees(component->m_twistAngleLimits.y);
    }

    static glm::vec2 coneAngle{0, 0};
    if (ImGui::InputFloat2("Cone half angle (X,Z)", reinterpret_cast<float *>(&coneAngle))) {
        component->m_coneHalfAngle.x = glm::radians(coneAngle.x);
        component->m_coneHalfAngle.y = glm::radians(coneAngle.y);
    } else {
        coneAngle.x = glm::degrees(component->m_coneHalfAngle.x);
        coneAngle.y = glm::degrees(component->m_coneHalfAngle.y);
    }

    ImGui::Checkbox("Swing motor settings", &component->m_isSwingMotorSettingsEnabled);
    if (component->m_isSwingMotorSettingsEnabled) {
        ImGui::InputFloat("Damping", &component->m_swingMotorDamping);
        ImGui::InputFloat("Frequency", &component->m_swingMotorFrequency);
    }

    ImGui::Checkbox("Look at behaviour", &component->m_useLookAtBehaviour);
}
