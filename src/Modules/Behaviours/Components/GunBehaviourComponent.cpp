#include "GunBehaviourComponent.h"

GunBehaviourComponent::GunBehaviourComponent() : Component(), m_isActive(true) {
}

void GunBehaviourComponent::serialize(nlohmann::json &j) {
    j[ACTIVE_KEY] = m_isActive;
}

void GunBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_isActive = j.value(ACTIVE_KEY, m_isActive);
}