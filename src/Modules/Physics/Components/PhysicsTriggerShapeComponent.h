#pragma once

#include "../../../Core/API.h"
#include "PhysicsBodyComponent.h"

class PhysicsTriggerShapeComponent : public Component {
    TYPE_DEFINITION(PhysicsTriggerShapeComponent);

public:
    PhysicsTriggerShapeComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool isReady() override;

    void create(TransformComponent &transform);

    [[nodiscard]] bool isCreated() const;

    ResourceHandle<PhysicsResource> m_PhysicsResource;

    JPH::Body *m_physicsBody;
};
