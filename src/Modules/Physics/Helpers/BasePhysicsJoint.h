#pragma once

#include "../Components/PhysicsBodyComponent.h"

class BasePhysicsJoint : public PhysicsComponent {
public:
    inline static const std::string ENTITY_KEY_A = "targetEntityA";
    inline static const std::string ENTITY_KEY_B = "targetEntityB";

    BasePhysicsJoint() : PhysicsComponent() {
    }

    virtual ~BasePhysicsJoint() = default;

    virtual bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) = 0;

    virtual void release() = 0;

    virtual void update() = 0;

    [[nodiscard]] virtual float getUnitPosition() const {
        return 0;
    }

    virtual void activate() {
    }

    virtual void setMotorVelocity(float velocity) {
    }

    virtual void setMotorOff() const {
    }

    virtual void lockInPlace() {
    }

    virtual void unLock() {
    }

    static bool areAllowedToConnect(const PhysicsBodyComponent &bodyA, const PhysicsBodyComponent &bodyB) {
        if (!bodyA.isPhysicsCreated() || !bodyB.isPhysicsCreated()) {
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

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        m_targetEntityAName = j.value(ENTITY_KEY_A, "");
        m_targetEntityBName = j.value(ENTITY_KEY_B, "");

        resourceManager.request(m_PhysicsResource, "physics");
    }

    void serialize(nlohmann::json &j) override {
        j[ENTITY_KEY_A] = m_targetEntityAName;
        j[ENTITY_KEY_B] = m_targetEntityBName;
    }

    bool isReadyToCreate(EntityContext &ctx) const override {
        if (!m_PhysicsResource.isReady()) {
            return false;
        }

        const auto bodyB = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityBName);
        if (!bodyB || !bodyB->isPhysicsCreated()) {
            return false;
        }

        if (!m_targetEntityAName.empty()) {
            const auto bodyA = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityAName);
            if (!bodyA || !bodyA->isPhysicsCreated()) {
                return false;
            }
        } else {
            const auto bodyA = ctx.getEntityComponent<PhysicsBodyComponent>(m_EntityId.id());
            if (!bodyA || !bodyA->isPhysicsCreated()) {
                return false;
            }
        }

        return true;
    }

    void createPhysics(EntityContext &ctx) override {
        PhysicsBodyComponent *bodyA = nullptr;
        if (m_targetEntityAName.empty()) {
            bodyA = ctx.getEntityComponent<PhysicsBodyComponent>(m_EntityId.id());
        } else {
            bodyA = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityAName);
        }

        const auto bodyB = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityBName);

        if (bodyA && bodyB) {
            create(*bodyA, *bodyB);
        }
    }

    std::string m_targetEntityAName;
    std::string m_targetEntityBName;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
