#pragma once

#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../Components/PhysicsFixedJointComponent.h"

inline void processPhysicsFixedJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<PhysicsFixedJointComponent *>(c);

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *system.m_EntityContext,
        component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    );

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *system.m_EntityContext,
        component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    );
}
