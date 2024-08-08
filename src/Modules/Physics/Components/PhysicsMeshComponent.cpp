#include "PhysicsMeshComponent.h"

PhysicsMeshComponent::PhysicsMeshComponent() : Component() {
}

void PhysicsMeshComponent::serialize(nlohmann::json &j) {
}

void PhysicsMeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
}

void PhysicsMeshComponent::registerWithSystems(EntityContext &ctx) {
}
