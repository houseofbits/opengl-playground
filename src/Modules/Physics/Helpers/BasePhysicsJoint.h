#pragma once

#include "../Components/PhysicsBodyComponent.h"

class BasePhysicsJoint : public EntityLinkedComponent {
public:
    BasePhysicsJoint() : m_targetEntityName(), m_targetEntityId(0) {}

    virtual ~BasePhysicsJoint() = default;

    virtual void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) = 0;

    virtual void release() = 0;

    virtual void update() = 0;

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

    std::string m_targetEntityName;
    Identity::Type m_targetEntityId;
};
