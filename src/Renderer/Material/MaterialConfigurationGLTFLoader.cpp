#include "MaterialConfigurationGLTFLoader.h"
#include <fstream>
#include "../../Core/Json/Json.h"
#include "MaterialConfiguration.h"
#include "../../Core/Helper/File.h"

MaterialConfigurationGLTFLoader::MaterialConfigurationGLTFLoader() = default;

MaterialConfiguration MaterialConfigurationGLTFLoader::createFromGLTFMaterial(
    const tinygltf::Model &model, const tinygltf::Material &material) {

    MaterialConfiguration config;

    if (material.emissiveTexture.index >= 0) {
        config.emissiveTextureUri = getTextureUri(model, material.emissiveTexture.index);
    }
    if (material.normalTexture.index >= 0) {
        config.normalTextureUri = getTextureUri(model, material.normalTexture.index);
    }
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
        config.diffuseTextureUri = getTextureUri(model, material.pbrMetallicRoughness.baseColorTexture.index);
    }
    if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
        config.roughnessMetallicTextureUri = getTextureUri(model, material.pbrMetallicRoughness.metallicRoughnessTexture.index);
    }

    config.emissiveColor = {
        material.emissiveFactor[0],
        material.emissiveFactor[1],
        material.emissiveFactor[2],
    };

    config.diffuseColor = {
        material.pbrMetallicRoughness.baseColorFactor[0],
        material.pbrMetallicRoughness.baseColorFactor[1],
        material.pbrMetallicRoughness.baseColorFactor[2]
    };

    return config;
}

std::string MaterialConfigurationGLTFLoader::getTextureUri(const tinygltf::Model &model, int textureIndex) {
    if (const auto source = model.textures[textureIndex].source; source >= 0) {
        if (model.images.size() <= source) {
            return "";
        }
        if (!File::exists(model.images[source].uri)) {
            return "";
        }
        return model.images[source].uri;
    }

    return "";
}
