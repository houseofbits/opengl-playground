#include "DoorBehaviourComponent.h"

DoorBehaviourComponent::DoorBehaviourComponent() : Component(),
                                                   m_isInitiallyOpen(false),
                                                   m_state(STATE_CLOSING),
                                                   m_isFixedOnFinalState(false) {
}

void DoorBehaviourComponent::serialize(nlohmann::json &j) {
    j[INITIAL_STATE_KEY] = m_isInitiallyOpen;
    j[FIXED_KEY] = m_isFixedOnFinalState;
}

void DoorBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_isInitiallyOpen = j.value(INITIAL_STATE_KEY, m_isInitiallyOpen);
    m_isFixedOnFinalState = j.value(FIXED_KEY, m_isFixedOnFinalState);
    m_state = m_isInitiallyOpen ? STATE_OPENING : STATE_CLOSING;
}
