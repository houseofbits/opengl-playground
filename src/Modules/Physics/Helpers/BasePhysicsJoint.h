#pragma once

#include "../../../Core/API.h"
#include "../Components/PhysicsBodyComponent.h"

class BasePhysicsJoint : public Component {
public:
    inline static const std::string ENTITY_KEY_A = "targetEntityA";
    inline static const std::string ENTITY_KEY_B = "targetEntityB";
    inline static const std::string CONNECTED_KEY = "initiallyConnected";

    enum JointState {
        STATE_CONNECTED,
        STATE_DISCONNECTED,
        STATE_AWAITING_CONNECTION,
        STATE_AWAITING_DISCONNECTION
    };

    BasePhysicsJoint() : Component(), m_isInitiallyConnected(true), m_state(STATE_AWAITING_CONNECTION) {
    }

    ~BasePhysicsJoint() override = default;

    virtual bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) = 0;

    virtual void release() {
        setStateDisconnected();
    }

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
        m_isInitiallyConnected = j.value(CONNECTED_KEY, m_isInitiallyConnected);

        resourceManager.request(m_PhysicsResource, "physics");

        if (m_isInitiallyConnected) {
            requestConnectState();
        } else {
            requestDisconnectState();
        }
    }

    void serialize(nlohmann::json &j) override {
        j[ENTITY_KEY_A] = m_targetEntityAName;
        j[ENTITY_KEY_B] = m_targetEntityBName;
        j[CONNECTED_KEY] = m_isInitiallyConnected;
    }

    bool isReadyToCreate(EntityContext &ctx) const {
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

    void createPhysics(EntityContext &ctx) {
        if (!isReadyToCreate(ctx)) {
            return;
        }

        PhysicsBodyComponent *bodyA = nullptr;
        if (m_targetEntityAName.empty()) {
            bodyA = ctx.getEntityComponent<PhysicsBodyComponent>(m_EntityId.id());
        } else {
            bodyA = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityAName);
        }

        const auto bodyB = ctx.findEntityComponent<PhysicsBodyComponent>(m_targetEntityBName);

        if (bodyA && bodyB) {
            if (!areAllowedToConnect(*bodyA, *bodyB)) {
                return;
            }

            if (create(*bodyA, *bodyB)) {
                setStateConnected();

                bodyA->wakeUp();
                bodyB->wakeUp();
            }
        }
    }

    void setStateConnected() {
        m_state = STATE_CONNECTED;
    }

    void setStateDisconnected() {
        m_state = STATE_DISCONNECTED;
    }

    void requestDisconnectState() {
        m_state = STATE_AWAITING_DISCONNECTION;
    }

    void requestConnectState() {
        m_state = STATE_AWAITING_CONNECTION;
    }

    bool isStateConnected() const {
        return m_state == STATE_CONNECTED;
    }

    [[nodiscard]] JointState getState() const {
        return m_state;
    }

    bool m_isInitiallyConnected;
    std::string m_targetEntityAName;
    std::string m_targetEntityBName;
    ResourceHandle<PhysicsResource> m_PhysicsResource;

private:
    JointState m_state;
};
