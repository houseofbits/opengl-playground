#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"

class PhysicsSliderJointComponent : public Component, public BasePhysicsJoint  {
TYPE_DEFINITION(PhysicsSliderJointComponent);
public:
    inline static const std::string ENTITY_KEY = "targetEntity";

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
};
