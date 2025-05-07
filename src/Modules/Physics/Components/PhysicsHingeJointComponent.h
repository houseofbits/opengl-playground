#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "PhysicsBodyComponent.h"
#include "Jolt/Physics/Constraints/HingeConstraint.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "../Helpers/Behaviours/StatefulJointBehaviour.h"

class PhysicsHingeJointComponent : public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsHingeJointComponent);

public:
    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string ARE_LIMITS_ENABLED_KEY = "enableLimits";
    inline static const std::string ARE_LOCKING_LIMITS_ENABLED_KEY = "enableLockingLimits";
    inline static const std::string LIMITS_KEY = "limits";
    inline static const std::string ENABLE_MOTOR_SETTINGS_KEY = "enableMotorSettings";
    inline static const std::string MOTOR_MAX_FORCE_KEY = "motorMaxForce";
    inline static const std::string MOTOR_DAMPING_KEY = "motorDamping";
    inline static const std::string MOTOR_FREQUENCY_KEY = "motorFrequency";
    inline static const std::string ATTACHMENT_A_POSITION_KEY = "attachmentAPosition";
    inline static const std::string ATTACHMENT_A_ROTATION_KEY = "attachmentARotation";
    inline static const std::string ATTACHMENT_B_POSITION_KEY = "attachmentBPosition";
    inline static const std::string ATTACHMENT_B_ROTATION_KEY = "attachmentBRotation";
    inline static const std::string STATEFUL_BEHAVIOUR_KEY = "statefulBehaviour";

    PhysicsHingeJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool create(PhysicsComponent &bodyA, PhysicsComponent &bodyB) override;

    void release() override;

    void activate() override;

    void update() override;

    [[nodiscard]] float getUnitPosition() const override;

    void lockInPlace() override;

    void unLock() override;

    void setMotorVelocity(float velocity) override;

    void setMotorOff() const override;

    bool m_isLimitsEnabled;
    bool m_isLockingToLimitsEnabled;
    JPH::Ref<JPH::HingeConstraint> m_Joint;
    glm::vec2 m_angularLimits;
    bool m_isMotorSettingsEnabled;
    float m_motorForceLimit;
    float m_motorDamping;
    float m_motorFrequency;

    glm::mat4 m_localAttachmentMatrixA;
    glm::mat4 m_localAttachmentMatrixB;

    bool m_useStatefulJointBehaviour;
    StatefulJointBehaviour m_statefulJointBehaviour;
};
