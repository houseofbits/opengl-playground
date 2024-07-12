#pragma once

#include "../../../CoreV2/API.h"
#include "../Resources/MaterialResource.h"
#include "../Resources/StaticMeshResource.h"

class StaticMeshComponent : public Component {
    TYPE_DEFINITION(StaticMeshComponent);

public:
    inline static const std::string MODEL_KEY = "model";
    inline static const std::string MATERIAL_KEY = "material";

    StaticMeshComponent();

    void serialize(nlohmann::json &j) override {
        j[MODEL_KEY] = m_Mesh().m_Path;
        j[MATERIAL_KEY] = m_Material().m_Path;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string path = j.value(MODEL_KEY, m_Mesh().m_Path);
        resourceManager.request(m_Mesh, path);

        path = j.value(MATERIAL_KEY, m_Material().m_Path);
        resourceManager.request(m_Material, path);
    }

    void registerWithSystems(EntityContext &ctx) override;
    std::string getListName(Entity* e) {
        return e->m_Name + " " + " STATIC MESH";
    }

    ResourceHandle<StaticMeshResource> m_Mesh;
    ResourceHandle<MaterialResource> m_Material;
};
