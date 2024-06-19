#pragma once

#include "../../../CoreV2/API.h"
#include "../../../ResourcesV2/TextureResource.h"

class MaterialComponent : public Component {
public:
    inline static const std::string DIFFUSE_TEXTURE_KEY = "diffuseTexture";

    MaterialComponent();

    void serialize(nlohmann::json &j) override {
        j[DIFFUSE_TEXTURE_KEY] = m_Diffuse().m_Path;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string model = j.value(DIFFUSE_TEXTURE_KEY, m_Diffuse().m_Path);

        resourceManager.request(m_Diffuse, model);
    }

    void registerWithSystems(EntityContext &ctx) override;

    ResourceHandle<TextureResource> m_Diffuse;
};
