#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/SwingTwistConstraint.h"

class PhysicsSwingTwistJointComponent : public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsSwingTwistJointComponent);

    inline static const std::string ATTACHMENT_A_POSITION_KEY = "attachmentAPosition";
    inline static const std::string ATTACHMENT_A_ROTATION_KEY = "attachmentARotation";
    inline static const std::string ATTACHMENT_B_POSITION_KEY = "attachmentBPosition";
    inline static const std::string ATTACHMENT_B_ROTATION_KEY = "attachmentBRotation";
    inline static const std::string TWIST_LIMITS_KEY = "twistLimits";
    inline static const std::string CONE_HALF_ANGLES_KEY = "coneHalfAngles";

public:
    PhysicsSwingTwistJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    bool create(PhysicsComponent &bodyA, PhysicsComponent &bodyB) override;

    void update()  override;

    void release() override;

    JPH::Ref<JPH::SwingTwistConstraint> m_Joint;
    glm::mat4 m_localAttachmentMatrixA;
    glm::mat4 m_localAttachmentMatrixB;
    glm::vec2 m_twistAngleLimits;
    glm::vec2 m_coneHalfAngle;
};