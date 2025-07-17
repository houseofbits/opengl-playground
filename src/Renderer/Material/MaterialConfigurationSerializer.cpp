#include "MaterialConfigurationSerializer.h"
#include "MaterialConfiguration.h"
#include "../../Core//Helper/Log.h"
#include "../../Core/Json/Json.h"

MaterialConfigurationSerializer::MaterialConfigurationSerializer()
= default;

MaterialConfiguration MaterialConfigurationSerializer::deserialize(const nlohmann::json &json) {
    MaterialConfiguration config;
    config.diffuseColor = Json::getValue(json, KEY_DIFFUSE_COLOR, config.diffuseColor);
    config.emissiveColor = Json::getValue(json, KEY_EMISSIVE_COLOR, config.emissiveColor);
    config.roughnessFactor = Json::getValue(json, KEY_ROUGHNESS_FACTOR, config.roughnessFactor);
    config.metallicFactor = Json::getValue(json, KEY_METALLIC_FACTOR, config.metallicFactor);
    config.diffuseTextureUri = Json::getValue(json, KEY_DIFFUSE_TEXTURE, config.diffuseTextureUri);
    config.normalTextureUri = Json::getValue(json, KEY_NORMAL_TEXTURE, config.normalTextureUri);
    config.roughnessMetallicTextureUri = Json::getValue(json, KEY_ROUGHNESS_METALLIC_TEXTURE,
                                                                config.roughnessMetallicTextureUri);
    config.emissiveTextureUri = Json::getValue(json, KEY_EMISSIVE_TEXTURE, config.emissiveTextureUri);
    config.selfIllumination = Json::getValue(json, KEY_SELF_ILLUMINATION_FACTOR, config.selfIllumination);
    config.doesCastShadows = Json::getValue(json, KEY_DOES_CAST_SHADOWS, config.doesCastShadows);
    config.doesReceiveShadows = Json::getValue(json, KEY_DOES_RECEIVE_SHADOWS, config.doesReceiveShadows);

    return config;
}

void MaterialConfigurationSerializer::serialize(MaterialConfiguration &config, nlohmann::json &json) {
    json[KEY_DIFFUSE_COLOR] = config.diffuseColor;
    json[KEY_EMISSIVE_COLOR] = config.emissiveColor;
    json[KEY_ROUGHNESS_FACTOR] = config.roughnessFactor;
    json[KEY_METALLIC_FACTOR] = config.metallicFactor;
    json[KEY_SELF_ILLUMINATION_FACTOR] = config.selfIllumination;
    json[KEY_DOES_CAST_SHADOWS] = config.doesCastShadows;
    json[KEY_DOES_RECEIVE_SHADOWS] = config.doesReceiveShadows;

    if (!config.diffuseTextureUri.empty()) {
        json[KEY_DIFFUSE_TEXTURE] = config.diffuseTextureUri;
    } else {
        json[KEY_DIFFUSE_TEXTURE] = nullptr;
    }

    if (!config.normalTextureUri.empty()) {
        json[KEY_NORMAL_TEXTURE] = config.normalTextureUri;
    } else {
        json[KEY_NORMAL_TEXTURE] = nullptr;
    }

    if (!config.roughnessMetallicTextureUri.empty()) {
        json[KEY_ROUGHNESS_METALLIC_TEXTURE] = config.roughnessMetallicTextureUri;
    } else {
        json[KEY_ROUGHNESS_METALLIC_TEXTURE] = nullptr;
    }

    if (!config.emissiveTextureUri.empty()) {
        json[KEY_EMISSIVE_TEXTURE] = config.emissiveTextureUri;
    } else {
        json[KEY_EMISSIVE_TEXTURE] = nullptr;
    }
}
