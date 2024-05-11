#pragma once

#include "../../libs/tinygltf/json.hpp"
#include "../Renderer/RenderManager.h"
#include "Scene.h"
#include <glm/vec3.hpp>
#include <string>

class SceneLoader {
private:
    RenderManager *renderManager;

    static glm::vec3 getVec3FromJsonArray(nlohmann::json::array_t array);
    static bool readJsonData(const std::string &filename, nlohmann::json &data);

    void populateModel(ModelComponent &model, nlohmann::json &data);
    void populateLight(Light &light, nlohmann::json &data);
    void populateCamera(Camera &camera, nlohmann::json &data);

public:
    explicit SceneLoader(RenderManager *renderManager);
    bool loadSceneFromJson(const std::string &filename, Scene &scene);
};
