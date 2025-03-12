#include "GLTFFileLoader.h"
#include "../../Core/Helper/Log.h"

tinygltf::Model *GLTFFileLoader::loadModel(const std::string &path) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    const auto model = new tinygltf::Model();

    bool res = loader.LoadASCIIFromFile(model, &err, &warn, path);
    if (!warn.empty()) {
        Log::warn("WARN: ", warn);

        delete model;

        return nullptr;
    }

    if (!err.empty()) {
        Log::error("ERR: ", err);

        delete model;

        return nullptr;
    }

    if (!res) {
        Log::warn("Failed to load model: ", path);

        delete model;

        return nullptr;
    }

    return model;
}
