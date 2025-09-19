#pragma once

#include "../../../Core/API.h"
#include "../Resources/MaterialResource.h"
#include "../Resources/StaticMeshResource.h"

class StaticMeshComponent : public Component {
TYPE_DEFINITION(StaticMeshComponent);

public:
    enum TargetRenderPass {
        SOLID,
        SHADOW,
        // TRANSPARENT, TODO
    };
    inline static const std::string MODEL_KEY = "model";
    inline static const std::string MATERIAL_KEY = "material";
    inline static const std::string TARGET_RENDERER_KEY = "renderer";

    StaticMeshComponent();

    void serialize(nlohmann::json &j) override {
        j[MODEL_KEY] = m_Mesh().m_Path;
        j[MATERIAL_KEY] = m_Material().m_Path;
        j[TARGET_RENDERER_KEY] = m_TargetRenderNameMap[m_targetRenderer];
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string path = j.value(MODEL_KEY, m_Mesh().m_Path);
        resourceManager.request(m_Mesh, path);

        path = j.value(MATERIAL_KEY, m_Material().m_Path);
        resourceManager.request(m_Material, path);

        m_targetRenderer = getTargetRendererTypeFromName(
                j.value(TARGET_RENDERER_KEY, m_TargetRenderNameMap.begin()->second));
    }

    std::string getListName(Entity *e) {
        return e->m_Name + " " + " STATIC MESH";
    }

    TargetRenderPass getTargetRendererTypeFromName(const std::string &name) {
        for (const auto &[key, value]: m_TargetRenderNameMap) {
            if (value == name) {
                return key;
            }
        }

        return SOLID;
    }

    ResourceHandle<StaticMeshResource> m_Mesh;
    ResourceHandle<MaterialResource> m_Material;
    TargetRenderPass m_targetRenderer;
    std::unordered_map<TargetRenderPass, std::string> m_TargetRenderNameMap;
};
