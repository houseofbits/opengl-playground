#include "HingeJointComponent.h"

HingeJointComponent::HingeJointComponent() : Component(),
                                             m_PhysicsResource() {
}

void HingeJointComponent::serialize(nlohmann::json &j) {
}

void HingeJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void HingeJointComponent::registerWithSystems(EntityContext &ctx) {
}

bool HingeJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}
