#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/DoorBehaviourComponent.h"

inline void processDoorBehaviourComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<DoorBehaviourComponent *>(c);

    ImGui::Checkbox("Is initially open", &component->m_isInitiallyOpen);
    ImGui::Checkbox("Is fixed on final state", &component->m_isFixedOnFinalState);

    ImGui::InputFloat("Velocity", &component->m_velocity);

    // EntityLinkedComponentEdit::process(
    //     *system.m_EventManager,
    //     *system.m_EntityContext,
    //     body,
    //     "Trigger entity##DOOR_TRIGGER_ENTITY_NAME"
    // );
}