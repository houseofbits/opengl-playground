#pragma once

#include "../../../Core/API.h"

class PhysicsMeshComponent : public Component {
    TYPE_DEFINITION(PhysicsMeshComponent);
public:
    PhysicsMeshComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
};
