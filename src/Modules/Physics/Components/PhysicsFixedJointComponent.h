#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/FixedConstraint.h"

class PhysicsFixedJointComponent : public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsFixedJointComponent);

public:
    PhysicsFixedJointComponent();

    void release() override;

    void update() override;

    bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    JPH::FixedConstraint *m_Joint;
};
