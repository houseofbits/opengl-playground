#pragma once

#include "../../../libs/tinygltf/tiny_gltf.h"

class MaterialConfiguration;

class MaterialConfigurationGLTFLoader {
public:
    MaterialConfigurationGLTFLoader();

    static MaterialConfiguration createFromGLTFMaterial(const tinygltf::Model &model,
                                                        const tinygltf::Material &material);

private:
    static std::string getTextureUri(const tinygltf::Model &model, int textureIndex);
};
