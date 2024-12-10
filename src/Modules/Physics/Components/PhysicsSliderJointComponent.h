#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/SliderConstraint.h"

class PhysicsSliderJointComponent : public Component, public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsSliderJointComponent);

    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string AXIS_KEY = "axis";
    inline static const std::string ARE_LIMITS_ENABLED_KEY = "enableLimits";
    inline static const std::string LIMITS_KEY = "limits";
    inline static const std::string ENABLE_MOTOR_SETTINGS_KEY = "enableMotorSettings";
    inline static const std::string MOTOR_MAX_FORCE_KEY = "motorMaxForce";
    inline static const std::string MOTOR_DAMPING_KEY = "motorDamping";
    inline static const std::string MOTOR_FREQUENCY_KEY = "motorFrequency";

public:
    PhysicsSliderJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool isReady() override;

    void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    void release() override;

    [[nodiscard]] bool isCreated() const override;

    void update() override;

    float getUnitPosition() const;

    void activate();

    void setMotorVelocity(float velocity) const;

    void setMotorOff() const;

    void lockInPlace();

    void unLock();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    JPH::SliderConstraint *m_Joint;
    bool m_isLimitsEnabled;
    glm::vec2 m_limits;
    glm::vec3 m_axis;
    bool m_isMotorSettingsEnabled;
    float m_motorForceLimit;
    float m_motorDamping;
    float m_motorFrequency;
};
