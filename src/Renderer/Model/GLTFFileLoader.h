#pragma once

#include "../../../libs/tinygltf/tiny_gltf.h"

class GLTFFileLoader 
{
public:
    static tinygltf::Model* loadModel(const std::string& path);
};
