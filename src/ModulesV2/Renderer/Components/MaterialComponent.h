#pragma once

#include "../../../CoreV2/API.h"
#include "../../../ResourcesV2/TextureResource.h"

class MaterialComponent : public Component {
public:
    inline static const std::string DIFFUSE_TEXTURE_KEY = "diffuseTexture";
    inline static const std::string NORMAL_TEXTURE_KEY = "normalTexture";

    MaterialComponent();

    void serialize(nlohmann::json &j) override {
        j[DIFFUSE_TEXTURE_KEY] = m_Diffuse().m_Path;
        j[NORMAL_TEXTURE_KEY] = m_Normal().m_Path;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string filename = j.value(DIFFUSE_TEXTURE_KEY, m_Diffuse().m_Path);
        resourceManager.request(m_Diffuse, filename);

        filename = j.value(NORMAL_TEXTURE_KEY, m_Normal().m_Path);
        resourceManager.request(m_Normal, filename);
    }

    void registerWithSystems(EntityContext &ctx) override;

    ResourceHandle<TextureResource> m_Diffuse;
    ResourceHandle<TextureResource> m_Normal;
};
