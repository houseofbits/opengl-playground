#include "PhysicsTriggerShapeComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

using namespace JPH;

PhysicsTriggerShapeComponent::PhysicsTriggerShapeComponent() : Component(),
                                                               m_physicsBody(nullptr) {
}

void PhysicsTriggerShapeComponent::serialize(nlohmann::json &j) {
}

void PhysicsTriggerShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsTriggerShapeComponent::isReadyToInitialize(EntityContext& ctx) const {
    return m_PhysicsResource.isReady();
}

void PhysicsTriggerShapeComponent::create(TransformComponent &transform) {
    m_physicsBody = PhysicsBuilder::newBody(m_PhysicsResource().getSystem())
            .setDebugColor({1, 0, 0})
            .setEntityReference(m_EntityId.id())
            .setTransform(transform)
            .addBoxShape(transform.getScale())
            .createStaticSensor();
 }

bool PhysicsTriggerShapeComponent::isCreated() const {
    return m_physicsBody != nullptr;
}
