#include "DoorBehaviourComponentEdit.h"

void DoorBehaviourComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    ImGui::Checkbox("Is initially open", &m_component->m_isInitiallyOpen);
    ImGui::Checkbox("Is fixed on final state", &m_component->m_isFixedOnFinalState);

    ImGui::InputFloat("Velocity", &m_component->m_velocity);

    // EntityLinkedComponentEdit::process(
    //     *system.m_EventManager,
    //     *system.m_EntityContext,
    //     body,
    //     "Trigger entity##DOOR_TRIGGER_ENTITY_NAME"
    // );
}

void DoorBehaviourComponentEdit::handleEntitySelection(Entity &entity, Component *component) {
    m_component = static_cast<DoorBehaviourComponent *>(component);
}
