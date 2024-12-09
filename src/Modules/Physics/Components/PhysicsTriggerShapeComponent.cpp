#include "PhysicsTriggerShapeComponent.h"

PhysicsTriggerShapeComponent::PhysicsTriggerShapeComponent() : Component(),
                                                               m_PhysicsResource() {

}

void PhysicsTriggerShapeComponent::serialize(nlohmann::json &j) {

}

void PhysicsTriggerShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {


    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsTriggerShapeComponent::isReady() {
    return m_PhysicsResource.isReady();
}