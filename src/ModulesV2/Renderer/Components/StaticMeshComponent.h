#pragma once

#include "../../../CoreV2/API.h"
#include "../../../ResourcesV2/StaticMeshResource.h"

class StaticMeshComponent : public Component {
public:
    inline static const std::string MODEL_KEY = "model";

    StaticMeshComponent();

    void serialize(nlohmann::json &j) override {
        j[MODEL_KEY] = m_Mesh().m_Path;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string model = j.value(MODEL_KEY, m_Mesh().m_Path);

        resourceManager.request(m_Mesh, model);
    }

    void registerWithSystems(EntityContext& ctx) override;

    ResourceHandle<StaticMeshResource> m_Mesh;
};
