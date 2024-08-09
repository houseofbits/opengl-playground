#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsMeshResource.h"

class PhysicsMeshComponent : public Component {
    TYPE_DEFINITION(PhysicsMeshComponent);
public:
    inline static const std::string MODEL_KEY = "model";

    PhysicsMeshComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;

    ResourceHandle<PhysicsMeshResource> m_triangleMesh;
};
