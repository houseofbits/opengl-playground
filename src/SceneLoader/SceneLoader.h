#pragma once

#include "../Components/Scene.h"
#include "../Renderer/RenderManager.h"
#include "ModelBuildConfiguration.h"
#include "ProbeBuildConfiguration.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

class SceneLoader {
private:
    RenderManager *renderManager;

    static glm::vec3 getVec3FromJsonArray(nlohmann::json::array_t array);
    static glm::vec4 getVec4FromJsonArray(nlohmann::json::array_t array);
    static bool readJsonData(const std::string &filename, nlohmann::json &data);

    void buildModel(ModelComponent &model, ModelBuildConfiguration& configuration);
    void buildProbe(EnvironmentProbe &probe, ProbeBuildConfiguration configuration);

    void populateLight(Light &light, nlohmann::json &data);
    void populateCamera(Camera &camera, nlohmann::json &data);
public:
    explicit SceneLoader(RenderManager *renderManager);
    bool loadSceneFromJson(const std::string &filename, Scene &scene);
};
