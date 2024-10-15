#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/SliderConstraint.h"

class PhysicsSliderJointComponent : public Component, public BasePhysicsJoint {
TYPE_DEFINITION(PhysicsSliderJointComponent);
public:
    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string AXIS_KEY = "axis";
    inline static const std::string ARE_LIMITS_ENABLED_KEY = "enableLimits";
    inline static const std::string LIMITS_KEY = "limits";

    PhysicsSliderJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void registerWithSystems(EntityContext &ctx) override;

    bool isReady() override;

    void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    void release() override;

    [[nodiscard]] bool isCreated() const override;

    void update() override;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    JPH::SliderConstraint *m_Joint;
    bool m_isLimitsEnabled;
    glm::vec2 m_limits;
    glm::vec3 m_axis;
};
