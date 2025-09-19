#include "MaterialConfigurationGLTFLoader.h"
#include "../../Core/Helper/File.h"
#include "../../Core/Helper/StringUtils.h"
#include "../../Core/Json/JsonTypes.h"
#include "MaterialConfiguration.h"
#include <fstream>

MaterialConfigurationGLTFLoader::MaterialConfigurationGLTFLoader() = default;

MaterialConfiguration MaterialConfigurationGLTFLoader::createFromGLTFMaterial(
    const tinygltf::Model &model, const tinygltf::Material &material,
    const std::string &basePath) {
  MaterialConfiguration config;

  if (material.emissiveTexture.index >= 0) {
    config.emissiveTextureUri =
        getTextureUri(model, material.emissiveTexture.index, basePath);
  }
  if (material.normalTexture.index >= 0) {
    config.normalTextureUri =
        getTextureUri(model, material.normalTexture.index, basePath);
  }
  if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
    config.diffuseTextureUri = getTextureUri(
        model, material.pbrMetallicRoughness.baseColorTexture.index, basePath);
  }
  if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
    config.roughnessMetallicTextureUri = getTextureUri(
        model, material.pbrMetallicRoughness.metallicRoughnessTexture.index,
        basePath);
  }

  config.roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
  config.metallicFactor = material.pbrMetallicRoughness.metallicFactor;

  config.emissiveColor = {
      material.emissiveFactor[0],
      material.emissiveFactor[1],
      material.emissiveFactor[2],
  };

  config.diffuseColor = {material.pbrMetallicRoughness.baseColorFactor[0],
                         material.pbrMetallicRoughness.baseColorFactor[1],
                         material.pbrMetallicRoughness.baseColorFactor[2]};

  return config;
}

std::string
MaterialConfigurationGLTFLoader::getTextureUri(const tinygltf::Model &model,
                                               int textureIndex,
                                               const std::string &basePath) {
  if (const auto source = model.textures[textureIndex].source; source >= 0) {
    if (model.images.size() <= source) {
      return "";
    }

    std::string textureUri = model.images[source].uri;

    if (!basePath.empty()) {
      std::string sep(1, std::filesystem::path::preferred_separator);

      textureUri = StringUtils::trimSlashes(basePath) + sep +
                   StringUtils::trimSlashes(textureUri);
    }

    // Add error log if checking here for file existence.
    // if (!File::exists(textureUri)) {
    //     return "";
    // }

    return textureUri;
  }

  return "";
}
