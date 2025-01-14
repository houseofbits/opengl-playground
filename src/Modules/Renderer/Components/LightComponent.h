#pragma once

#include "../../../Core/API.h"
#include "../Resources/ShadowMapResource.h"
#include "../Resources/TextureResource.h"

class LightComponent : public Component {
    TYPE_DEFINITION(LightComponent);

public:
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string ENABLED_KEY = "isEnabled";
    inline static const std::string COLOR_KEY = "color";
    inline static const std::string INTENSITY_KEY = "intensity";
    inline static const std::string BEAM_ANGLE_KEY = "beamAngle";
    inline static const std::string ATTENUATION_KEY = "attenuation";
    inline static const std::string RADIUS_KEY = "radius";
    inline static const std::string PROJECTION_TEXTURE_KEY = "projectionTexture";
    inline static const std::string CAST_SHADOWS_KEY = "castShadows";
    inline static const std::string SHADOW_RESOLUTION_KEY = "shadowResolution";
    inline static const std::string SHADOW_BIAS_KEY = "shadowBias";

    enum Type {
        OMNI = 0,
        SPOT = 1,
        DIRECT = 2,
    };

    LightComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    std::string getLightTypeName();
    Type getLightTypeFromName(const std::string &name);
    void prepareShadowMapResources(ResourceManager &resourceManager);
    int getNumberOfBufferLights() const;
    std::string getListName(Entity* e) {
        return e->m_Name + " " + m_TypeNameMap[m_Type] + " LIGHT";
    }
    void resizeShadowMaps(int size);

    bool m_isEnabled;
    std::unordered_map<Type, std::string> m_TypeNameMap;
    Type m_Type;
    glm::vec3 m_Color;
    float m_Intensity;
    float m_beamAngle;
    float m_Attenuation;
    float m_Radius;
    bool m_doesCastShadows;
    int m_shadowResolution;
    float m_shadowBias;
    ResourceHandle<TextureResource> m_Projection;
    std::vector<ResourceHandle<ShadowMapResource>*> m_ShadowMaps;
    std::vector<int> m_lightBufferIndices;

private:
    std::string getShadowMapResourceName(int viewIndex);
    void removeShadowMaps();
};
