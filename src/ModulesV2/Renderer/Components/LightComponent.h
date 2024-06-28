#pragma once

#include "../../../CoreV2/API.h"
#include "../../../ResourcesV2/TextureResource.h"

class LightComponent : public Component {
public:
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string COLOR_KEY = "color";
    inline static const std::string INTENSITY_KEY = "intensity";
    inline static const std::string BEAM_ANGLE_KEY = "beamAngle";
    inline static const std::string ATTENUATION_KEY = "attenuation";
    inline static const std::string PROJECTION_TEXTURE_KEY = "projectionTexture";

    enum Type {
        OMNI = 0,
        SPOT = 1,
        DIRECT = 2,
    };

    LightComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    std::string getTypeName();
    Type getTypeFromName(const std::string &name);

    std::unordered_map<Type, std::string> m_TypeNameMap;
    Type m_Type;
    glm::vec3 m_Color;
    float m_Intensity;
    float m_beamAngle;
    float m_Attenuation;
    ResourceHandle<TextureResource> m_Projection;
};
