#pragma once

#include "StatefulJointBehaviourEdit.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../../Core/Helper/Math.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../Components/PhysicsDistanceJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processPhysicsDistanceJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsDistanceJointComponent *>(c);

    ImGui::Checkbox("Is initially connected", &component->m_isInitiallyConnected);
    //
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
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, false, false);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, false, false);
        ImGui::TreePop();
    }

    ImGui::InputFloat2("Limits", &component->m_limits[0]);
}
