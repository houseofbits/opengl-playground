#pragma once
#include <string>
#include "../../Core/Json/JsonTypes.h"

class MaterialConfiguration;

class MaterialConfigurationSerializer {
private:
    inline static const std::string KEY_DIFFUSE_COLOR = "diffuseColor";
    inline static const std::string KEY_EMISSIVE_COLOR = "emiSsiveColor";
    inline static const std::string KEY_ROUGHNESS_FACTOR = "roughnessFactor";
    inline static const std::string KEY_METALLIC_FACTOR = "metallicFactor";
    inline static const std::string KEY_DIFFUSE_TEXTURE = "diffuseTexture";
    inline static const std::string KEY_NORMAL_TEXTURE = "normalTexture";
    inline static const std::string KEY_ROUGHNESS_METALLIC_TEXTURE = "roughnessMetallicTexture";
    inline static const std::string KEY_EMISSIVE_TEXTURE = "emissiveTexture";
    inline static const std::string KEY_SELF_ILLUMINATION_FACTOR = "selfIlluminationFactor";
    inline static const std::string KEY_DOES_CAST_SHADOWS = "doesCastShadows";
    inline static const std::string KEY_DOES_RECEIVE_SHADOWS = "doesReceiveShadows";

public:
    MaterialConfigurationSerializer();

    static MaterialConfiguration deserialize(const nlohmann::json &json);

    static void serialize(MaterialConfiguration &config, nlohmann::json &json);
};
