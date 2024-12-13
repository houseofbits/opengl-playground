#pragma once

#include "../Components/PhysicsBodyComponent.h"

class BasePhysicsJoint : public EntityLinkedComponent {
public:
    BasePhysicsJoint() {
    }

    virtual ~BasePhysicsJoint() = default;

    virtual void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) = 0;

    virtual void release() = 0;

    virtual void update() = 0;

    virtual float getUnitPosition() const {
        return 0;
    }

    virtual void activate() {
    }

    virtual void setMotorVelocity(float velocity) const {
    }

    virtual void setMotorOff() const {
    }

    virtual void lockInPlace() {
    }

    virtual void unLock() {
    }

    [[nodiscard]] virtual bool isCreated() const = 0;

    static bool areAllowedToConnect(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
        if (!bodyA.isCreated() || !bodyB.isCreated()) {
            return false;
        }

        if (bodyA.m_EntityId == bodyB.m_EntityId) {
            return false;
        }

        if (bodyA.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC &&
            bodyB.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC) {
            return false;
        }

        return true;
    }
};
