#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/DistanceConstraint.h"

class PhysicsDistanceJointComponent : public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsDistanceJointComponent);

    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string LIMITS_KEY = "limits";
    inline static const std::string ATTACHMENT_A_POSITION_KEY = "attachmentAPosition";
    inline static const std::string ATTACHMENT_B_POSITION_KEY = "attachmentBPosition";

public:
    PhysicsDistanceJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    void release() override;

    void update() override;

    bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    JPH::Ref<JPH::DistanceConstraint> m_Joint;
    glm::mat4 m_localAttachmentMatrixA;
    glm::mat4 m_localAttachmentMatrixB;
    glm::vec2 m_limits;
};