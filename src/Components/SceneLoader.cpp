#include "SceneLoader.h"
#include <fstream>

SceneLoader::SceneLoader(RenderManager *manager) : renderManager(manager) {
}

bool SceneLoader::loadSceneFromJson(const std::string &filename, Scene &scene) {
    nlohmann::json data;
    if (readJsonData(filename, data)) {
        scene.clear();

        for (auto &meshData: data["meshes"]) {
            populateModel(scene.createModel(), meshData);
        }

        for (auto &lightData: data["lights"]) {
            populateLight(scene.createLight(), lightData);
        }
    }

    return false;
}

void SceneLoader::populateModel(ModelComponent &model, nlohmann::json &meshData) {

    if (meshData["model"] != nullptr) {
        model.model.createFromFile(meshData["model"]);
    }
    if (meshData["diffuseTexture"] != nullptr) {
        model.diffuseTextureId = renderManager->atlasManager.loadTextureIntoAtlas(meshData["diffuseTexture"], TextureAtlasManager::ATLAS_DIFFUSE);
    }
    if (meshData["normalTexture"] != nullptr) {
        model.normalTextureId = renderManager->atlasManager.loadTextureIntoAtlas(meshData["normalTexture"], TextureAtlasManager::ATLAS_NORMALS);
    }
    if (meshData["position"] != nullptr) {
        model.setPosition(getVec3FromJsonArray(meshData["position"]));
    }
    if (meshData["rotation"] != nullptr) {
        model.setRotation(getVec3FromJsonArray(meshData["rotation"]));
    }
    if (meshData["scale"] != nullptr) {
        model.setScale(getVec3FromJsonArray(meshData["scale"]));
    }
}

void SceneLoader::populateLight(Light &light, nlohmann::json &lightData) {
    if (lightData["doesCastShadows"] != nullptr) {
        light.doesCastShadows = lightData["doesCastShadows"];
    }

    std::string type = lightData["type"];
    if (type == "POINT") {
        light.type = Light::POINT;
    }
    if (type == "SPOT") {
        light.type = Light::SPOT;
    }

    if (lightData["projectionTexture"] != nullptr) {
        light.projectionTextureId = renderManager->atlasManager.loadTextureIntoAtlas(lightData["projectionTexture"], TextureAtlasManager::ATLAS_EFFECTS);
    }
    if (lightData["position"] != nullptr) {
        light.position = getVec3FromJsonArray(lightData["position"]);
    }
    if (lightData["direction"] != nullptr) {
        light.direction = getVec3FromJsonArray(lightData["direction"]);
    }
    if (lightData["color"] != nullptr) {
        light.color = getVec3FromJsonArray(lightData["color"]);
    }
    if (lightData["beamAngle"] != nullptr) {
        light.beamAngle = lightData["beamAngle"];
    }
    if (lightData["distAttenMax"] != nullptr) {
        light.distAttenMax = lightData["distAttenMax"];
    }
    if (lightData["intensity"] != nullptr) {
        light.intensity = lightData["intensity"];
    }
}

bool SceneLoader::readJsonData(const std::string &filename, nlohmann::json &data) {
    std::ifstream file(filename);
    if (file.fail()) {
        std::cout << "Scene file not found " << filename << std::endl;
        return false;
    }

    data = nlohmann::json::parse(file);

    return true;
}

glm::vec3 SceneLoader::getVec3FromJsonArray(nlohmann::json::array_t array) {
    glm::vec3 value(0.0);

    value.x = array[0];
    value.y = array[1];
    value.z = array[2];

    return value;
}