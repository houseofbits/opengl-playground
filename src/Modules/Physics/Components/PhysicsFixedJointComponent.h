#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/FixedConstraint.h"

class PhysicsFixedJointComponent : public BasePhysicsJoint {
    TYPE_DEFINITION(PhysicsFixedJointComponent);

    inline static const std::string ATTACHMENT_A_POSITION_KEY = "attachmentAPosition";
    inline static const std::string ATTACHMENT_B_POSITION_KEY = "attachmentBPosition";

public:
    PhysicsFixedJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    void release() override;

    void update() override;

    bool create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    JPH::FixedConstraint *m_Joint;
    glm::mat4 m_localAttachmentMatrixA;
    glm::mat4 m_localAttachmentMatrixB;
};
