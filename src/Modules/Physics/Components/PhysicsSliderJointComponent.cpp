#include "PhysicsSliderJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/JointsProcessingSystem.h"

PhysicsSliderJointComponent::PhysicsSliderJointComponent() : Component(),
                                                             BasePhysicsJoint(),
                                                             m_PhysicsResource() {

}

void PhysicsSliderJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
}

void PhysicsSliderJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsSliderJointComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<JointsProcessingSystem>(this);
}

bool PhysicsSliderJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsSliderJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {

}

void PhysicsSliderJointComponent::release() {

}

bool PhysicsSliderJointComponent::isCreated() const {
    return false;
}

void PhysicsSliderJointComponent::update() {

}
