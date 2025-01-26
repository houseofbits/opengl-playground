#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/FixedConstraint.h"

class PhysicsFixedJointComponent : public Component, public BasePhysicsJoint {
TYPE_DEFINITION(PhysicsFixedJointComponent);
public:
    inline static const std::string ENTITY_KEY = "targetEntity";

    PhysicsFixedJointComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool areResourcesReady() const override;

    void create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) override;

    void release() override;

    [[nodiscard]] bool isCreated() const override;

    void update() override;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    JPH::FixedConstraint *m_Joint;
};
