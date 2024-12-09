#include "PhysicsFixedJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

PhysicsFixedJointComponent::PhysicsFixedJointComponent() : Component(),
                                                           BasePhysicsJoint(),
                                                           m_PhysicsResource(),
                                                           m_Joint(nullptr) {

}

void PhysicsFixedJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
}

void PhysicsFixedJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);

    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsFixedJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsFixedJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return;
    }

    JPH::FixedConstraintSettings settings;
    settings.mAutoDetectPoint = true;
    m_Joint = (JPH::FixedConstraint *) settings.Create(*bodyA.m_physicsBody, *bodyB.m_physicsBody);

    m_PhysicsResource().getSystem().AddConstraint(m_Joint);

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
