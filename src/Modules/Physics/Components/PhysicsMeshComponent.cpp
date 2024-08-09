#include "PhysicsMeshComponent.h"

PhysicsMeshComponent::PhysicsMeshComponent() : Component(), m_triangleMesh() {
}

void PhysicsMeshComponent::serialize(nlohmann::json &j) {
    j[MODEL_KEY] = m_triangleMesh().m_Path;
}

void PhysicsMeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    std::string path = j.value(MODEL_KEY, m_triangleMesh().m_Path);
    resourceManager.request(m_triangleMesh, path);
}

void PhysicsMeshComponent::registerWithSystems(EntityContext &ctx) {
}
