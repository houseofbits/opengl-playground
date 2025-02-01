#include "PhysicsFixedJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsFixedJointComponent::PhysicsFixedJointComponent() : BasePhysicsJoint(),
                                                           m_Joint(nullptr) {
}

bool PhysicsFixedJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return false;
    }

    m_Joint = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setBodies(bodyA, bodyB)
            .createFixedConstraint();

    bodyA.wakeUp();
    bodyB.wakeUp();

    return true;
}

void PhysicsFixedJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        m_Joint = nullptr;
    }
}

void PhysicsFixedJointComponent::update() {
}
