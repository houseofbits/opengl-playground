#pragma once

#include "../../../Core/API.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../Components/PhysicsJointAttachmentComponent.h"

class BasePhysicsJoint : public Component {
public:
    inline static const std::string TARGET_ENTITY_KEY = "targetEntity";
    inline static const std::string TARGET_ENTITY_COMPONENT_KEY = "targetEntityComponent";
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

    virtual bool create(PhysicsComponent &bodyA, PhysicsComponent &bodyB,
                        PhysicsJointAttachmentComponent *attachmentB) = 0;

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

    static bool areAllowedToConnect(const PhysicsComponent &bodyA, const PhysicsComponent &bodyB) {
        if (bodyA.m_EntityId == bodyB.m_EntityId) {
            return false;
        }

        if (!bodyA.isDynamic() && !bodyB.isDynamic()) {
            return false;
        }

        return true;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        m_targetEntityName = j.value(TARGET_ENTITY_KEY, "");
        m_targetComponentName = j.value(TARGET_ENTITY_COMPONENT_KEY, "");
        m_isInitiallyConnected = j.value(CONNECTED_KEY, m_isInitiallyConnected);

        resourceManager.request(m_PhysicsResource, "physics");

        if (m_isInitiallyConnected) {
            requestConnectState();
        } else {
            requestDisconnectState();
        }
    }

    void serialize(nlohmann::json &j) override {
        j[TARGET_ENTITY_KEY] = m_targetEntityName;
        j[TARGET_ENTITY_COMPONENT_KEY] = m_targetComponentName;
        j[CONNECTED_KEY] = m_isInitiallyConnected;
    }

    bool isReadyToCreate(EntityContext &ctx) const {
        if (!m_PhysicsResource.isReady()) {
            return false;
        }

        const auto bodyA = ctx.getEntityComponent<PhysicsComponent>(m_EntityId.id());
        if (!bodyA || !bodyA->isPhysicsCreated()) {
            return false;
        }

        const auto bodyB = ctx.findEntityComponent<PhysicsComponent>(m_targetEntityName);
        if (!bodyB || !bodyB->isPhysicsCreated()) {
            return false;
        }

        return true;
    }

    void createPhysics(EntityContext &ctx) {
        if (!isReadyToCreate(ctx)) {
            Log::warn("Joint not ready to be created");

            return;
        }

        auto *bodyA = ctx.getEntityComponent<PhysicsComponent>(m_EntityId.id());;

        const auto entityB = ctx.findEntity(m_targetEntityName);
        if (!entityB) {
            Log::warn("Joint target entity not found");

            return;
        }

        const auto bodyB = entityB->getComponent<PhysicsComponent>();

        if (!bodyA) {
            Log::warn("Joint Self component not found");

            return;
        }

        if (!bodyB) {
            Log::warn("Joint body not found ", m_targetEntityName);

            return;
        }

        if (!areAllowedToConnect(*bodyA, *bodyB)) {
            Log::warn("Joint bodies are not allowed to connect");

            return;
        }

        auto attachment = entityB->getComponent<PhysicsJointAttachmentComponent>(m_targetComponentName);
        if (create(*bodyA, *bodyB, attachment)) {
            setStateConnected();

            bodyA->wakeUp();
            bodyB->wakeUp();
        }
    }

    void removeJoint(JPH::Constraint *joint) {
        if (joint != nullptr) {
            bool exists = false;
            auto constr = m_PhysicsResource().getSystem().GetConstraints();
            for (auto &constraint: constr) {
                if (constraint.GetPtr() == joint) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                m_PhysicsResource().getSystem().RemoveConstraint(joint);
            }
        }
    }

    void connectToEntity(const std::string &entityName) {
        m_targetEntityName = entityName;
        m_targetComponentName = "";

        requestConnectState();
    }

    void connectToEntityTarget(const std::string &entityName, const std::string &componentName) {
        m_targetEntityName = entityName;
        m_targetComponentName = componentName;

        requestConnectState();
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

    [[nodiscard]] bool isStateConnected() const {
        return m_state == STATE_CONNECTED;
    }

    [[nodiscard]] bool isStateDisconnected() const {
        return m_state == STATE_DISCONNECTED;
    }

    [[nodiscard]] JointState getState() const {
        return m_state;
    }

    bool m_isInitiallyConnected;
    std::string m_targetEntityName;
    std::string m_targetComponentName;
    ResourceHandle<PhysicsResource> m_PhysicsResource;

private:
    JointState m_state;
};
