#include "PhysicsFixedJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsFixedJointComponent::PhysicsFixedJointComponent() : Component(),
                                                           BasePhysicsJoint(),
                                                           m_PhysicsResource(),
                                                           m_Joint(nullptr) {
}

void PhysicsFixedJointComponent::serialize(nlohmann::json &j) {
    serializeLinkedEntity(j);
}

void PhysicsFixedJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    deserializeLinkedEntity(j);

    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsFixedJointComponent::areResourcesReady() const {
    return m_PhysicsResource.isReady();
}

void PhysicsFixedJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return;
    }

    m_Joint = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setBodies(bodyA, bodyB)
            .createFixedConstraint();

    bodyA.wakeUp();
    bodyB.wakeUp();
}

void PhysicsFixedJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        delete m_Joint;
        m_Joint = nullptr;
    }
}

bool PhysicsFixedJointComponent::isCreated() const {
    return m_Joint != nullptr;
}

void PhysicsFixedJointComponent::update() {
}
