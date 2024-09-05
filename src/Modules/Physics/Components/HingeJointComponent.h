#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"

class HingeJointComponent : public Component  {
public:
    HingeJointComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
