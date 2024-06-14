#include "SceneLoader.h"
#include "ProbeBuildConfiguration.h"
#include <fstream>

SceneLoader::SceneLoader(RenderManager *manager) : renderManager(manager) {
}

bool SceneLoader::loadSceneFromJson(const std::string &filename, Scene &scene) {
    nlohmann::json data;
    if (readJsonData(filename, data)) {
        scene.clear();

        for (auto &modelJson: data["meshes"]) {
//            ModelBuildConfiguration configuration = modelJson.get<ModelBuildConfiguration>();
//            ModelComponent &model = scene.createModel();
//            buildModel(model, configuration);
        }

        for (auto &lightData: data["lights"]) {
            populateLight(scene.createLight(), lightData);
        }

        if (data["camera"] != nullptr) {
            populateCamera(scene.camera, data["camera"]);
        }

        for (auto &probeJson: data["probes"]) {
//            ProbeBuildConfiguration configuration = probeJson.get<ProbeBuildConfiguration>();
//            auto &probe = scene.createProbe();
//            buildProbe(probe, configuration);
        }

        scene.filename = filename;

        return true;
    }

    return false;
}

void SceneLoader::buildModel(ModelComponent &modelComponent, ModelBuildConfiguration &configuration) {

    modelComponent.load(configuration.model);

    if (configuration.diffuseTexture.has_value()) {
        modelComponent.diffuseTextureId = renderManager->atlasManager.loadTextureIntoAtlas(
                configuration.diffuseTexture.value(), TextureAtlasManager::ATLAS_DIFFUSE);
    }

    if (configuration.normalTexture.has_value()) {
        modelComponent.normalTextureId = renderManager->atlasManager.loadTextureIntoAtlas(
                configuration.normalTexture.value(), TextureAtlasManager::ATLAS_NORMALS);
    }

    if (configuration.specularTexture.has_value()) {
        modelComponent.specularTextureId = renderManager->atlasManager.loadTextureIntoAtlas(
                configuration.specularTexture.value(), TextureAtlasManager::ATLAS_DIFFUSE);
    }

    if (configuration.position.has_value()) {
        modelComponent.setPosition(configuration.position.value());
    }

    if (configuration.rotation.has_value()) {
        modelComponent.setRotation(configuration.rotation.value());
    }

    if (configuration.scale.has_value()) {
        modelComponent.setScale(configuration.scale.value());
    }

    modelComponent.diffuseColor = configuration.diffuseColor.value_or(glm::vec3(1, 1, 1));
    modelComponent.specularColor = configuration.specularColor.value_or(glm::vec3(1, 1, 1));
    modelComponent.specularPower = configuration.specularPower.value_or(1);
    modelComponent.roughness = configuration.roughness.value_or(0);
    modelComponent.doesReceiveShadows = configuration.doesReceiveShadows.value_or(true);
    modelComponent.doesCastShadows = configuration.doesCastShadows.value_or(true);
    modelComponent.isSolidMaterial = configuration.isSolidMaterial.value_or(true);
    modelComponent.selfIllumination = configuration.selfIllumination.value_or(0);
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

void SceneLoader::populateCamera(Camera &camera, nlohmann::json &data) {
    if (data["position"] != nullptr) {
        camera.setPosition(getVec3FromJsonArray(data["position"]));
    }

    if (data["verticalAngle"] != nullptr && data["horizontalAngle"] != nullptr) {
        camera.setAngles(data["horizontalAngle"], data["verticalAngle"]);
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

glm::vec4 SceneLoader::getVec4FromJsonArray(nlohmann::json::array_t array) {
    glm::vec4 value(0.0);

    value.x = array[0];
    value.y = array[1];
    value.z = array[2];
    value.w = array[3];

    return value;
}
void SceneLoader::buildProbe(EnvironmentProbe &probe, ProbeBuildConfiguration configuration) {
    probe.position = configuration.position;
    probe.boundingBoxMin = configuration.boundingBoxMin;
    probe.boundingBoxMax = configuration.boundingBoxMax;
}
