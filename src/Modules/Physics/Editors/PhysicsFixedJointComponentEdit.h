#pragma once

#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../Components/PhysicsFixedJointComponent.h"

inline void processPhysicsFixedJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<PhysicsFixedJointComponent *>(c);

    ImGui::Checkbox("Is initially connected", &component->m_isInitiallyConnected);

    EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
        *system.m_EntityContext,
        component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    );

    EntityLinkedComponentEdit::processMulti<PhysicsBodyComponent, PhysicsCharacterComponent>(
        *system.m_EntityContext,
        component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    );

    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, false, false);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, false, false);
        ImGui::TreePop();
    }
}
