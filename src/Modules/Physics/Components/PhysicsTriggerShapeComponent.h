#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "Jolt/Physics/Constraints/FixedConstraint.h"

class PhysicsTriggerShapeComponent : public Component {
TYPE_DEFINITION(PhysicsTriggerShapeComponent);
public:
    PhysicsTriggerShapeComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool isReady() override;

    ResourceHandle<PhysicsResource> m_PhysicsResource;

};
