#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "PhysicsBodyComponent.h"
#include "Jolt/Physics/Constraints/HingeConstraint.h"
#include "../Helpers/BasePhysicsJoint.h"

class PhysicsHingeJointComponent : public Component, public ComponentTransformEdit, public BasePhysicsJoint {
TYPE_DEFINITION(PhysicsHingeJointComponent);
public:
    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string ARE_LIMITS_ENABLED_KEY = "enableLimits";
    inline static const std::string ARE_LOCKING_LIMITS_ENABLED_KEY = "enableLockingLimits";
    inline static const std::string AXIS_A_KEY = "axisA";
    inline static const std::string AXIS_B_KEY = "axisB";
    inline static const std::string ATTACHMENT_A_KEY = "attachmentA";
    inline static const std::string ATTACHMENT_B_KEY = "attachmentB";
    inline static const std::string LIMITS_KEY = "limits";

    PhysicsHingeJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool isReady() override;

    void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    void release() override;

    void activate();

    void update() override;

    [[nodiscard]] bool isCreated() const override;

    glm::mat4 getEditorTransform() override;

    void setFromEditorTransform(const glm::mat4 &) override;

    bool m_isLimitsEnabled;
    bool m_isLockingToLimitsEnabled;
    JPH::HingeConstraint *m_Joint;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    glm::vec2 m_angularLimits;
    glm::vec3 m_axisA;
    glm::vec3 m_axisB;
    glm::vec3 m_localAttachmentA;
    glm::vec3 m_localAttachmentB;

private:
    enum MovementState {
        UNKNOWN,
        CLOSED,
        OPEN,
        CLOSING,
        OPENING,
    };

    void open();

    void close();

    MovementState m_moveState;

};
