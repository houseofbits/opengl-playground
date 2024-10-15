#include "PhysicsTriggerShapeComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

PhysicsTriggerShapeComponent::PhysicsTriggerShapeComponent() : Component(),
                                                               m_PhysicsResource() {

}

void PhysicsTriggerShapeComponent::serialize(nlohmann::json &j) {

}

void PhysicsTriggerShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {


    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsTriggerShapeComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

bool PhysicsTriggerShapeComponent::isReady() {
    return m_PhysicsResource.isReady();
}