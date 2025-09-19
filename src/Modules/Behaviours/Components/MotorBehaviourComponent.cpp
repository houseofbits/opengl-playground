#include "MotorBehaviourComponent.h"

MotorBehaviourComponent::MotorBehaviourComponent() : Component() {
}

void MotorBehaviourComponent::serialize(nlohmann::json &j) {
    // j[SOME_KEY] = m_value;
}

void MotorBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    // m_value = j.value(SOME_KEY, m_value);
}