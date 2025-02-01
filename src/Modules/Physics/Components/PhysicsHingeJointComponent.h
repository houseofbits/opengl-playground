#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "PhysicsBodyComponent.h"
#include "Jolt/Physics/Constraints/HingeConstraint.h"
#include "../Helpers/BasePhysicsJoint.h"

class PhysicsHingeJointComponent : public ComponentTransformEdit, public BasePhysicsJoint {
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
    inline static const std::string ENABLE_MOTOR_SETTINGS_KEY = "enableMotorSettings";
    inline static const std::string MOTOR_MAX_FORCE_KEY = "motorMaxForce";
    inline static const std::string MOTOR_DAMPING_KEY = "motorDamping";
    inline static const std::string MOTOR_FREQUENCY_KEY = "motorFrequency";

    PhysicsHingeJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    void release() override;

    void activate();

    void update() override;

    glm::mat4 getWorldTransform() override;

    void setWorldTransform(const glm::mat4 &) override;

    float getUnitPosition() const override;

    void lockInPlace() override;

    void unLock() override;

    void setMotorVelocity(float velocity) const override;

    void setMotorOff() const override;

    bool m_isLimitsEnabled;
    bool m_isLockingToLimitsEnabled;
    JPH::HingeConstraint *m_Joint;
    glm::vec2 m_angularLimits;
    glm::vec3 m_axisA;
    glm::vec3 m_axisB;
    glm::vec3 m_localAttachmentA;
    glm::vec3 m_localAttachmentB;
    bool m_isMotorSettingsEnabled;
    float m_motorForceLimit;
    float m_motorDamping;
    float m_motorFrequency;
};
