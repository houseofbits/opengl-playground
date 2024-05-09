#include "Include.h"
#include <SDL2/SDL_opengl.h>
#include "Renderer/Texture2D.h"
#include "../libs/tinygltf/json.hpp"
#include <fstream>
#include "Events/InputEvent.h"

Entry::Entry() : window(&eventManager), renderManager(&window), debugMode(0)
{
    eventManager.registerEventReceiver(this, &Entry::handleInputEvent);
}

void Entry::run()
{
    window.create();

    init();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glm::mat4 planeTransform(1.0);
    planeTransform = glm::translate(planeTransform, glm::vec3(0, 1, 0));

    while (true)
    {
        if (!window.pollEvents() || !eventManager.processEvents())
        {
            break;
        }

        (*scene.lights.begin())->position = scene.camera.getPosition() - glm::vec3(0, 20, 0);
        (*scene.lights.begin())->direction = scene.camera.getDirection();

        renderManager.render(scene);

        if (debugMode > 0) {
            renderManager.renderDebug(debugMode);
        }

        window.doubleBuffer();

        // Animate point light
        // animatedLightAngle += Time::frameTime * 3.0;
        // animatedLight->position.x = cos(animatedLightAngle) * 400;
        // animatedLight->position.z = sin(animatedLightAngle) * 400;
    }

    window.destroy();
}

/**
 * TODO
 *
 * TODO: NICE TO HAVE
 *  - Refactor and optimize lighting fragment shader
 *  - Add shadow bias variables to light
 *  - Projection texture per light view
 *  - Optional light views for omni lights - configurable
 *
 *  Implement
 *  - Normal maps
 *  - Specular maps
 *  - PBR
 *  - Deferred rendering
 *  - SSAO
 *  - SSSM
 *  - Animations
 */
void Entry::init()
{
    renderManager.init();

//     loadSceneFromJson("resources/scenes/ducks-n-lights.json");
//     loadSceneFromJson("resources/scenes/multiple-spot-lights.json");
//    loadSceneFromJson("resources/scenes/single-spot-light.json");
//     loadSceneFromJson("resources/scenes/hall-with-columns.json");
     loadSceneFromJson("resources/scenes/hall-with-columns-omni.json");

     scene.camera.registerEventHandlers(&eventManager);
}

glm::vec3 getVec3FromJsonArray(nlohmann::json::array_t array)
{
    glm::vec3 value(0.0);

    value.x = array[0];
    value.y = array[1];
    value.z = array[2];

    return value;
}

void Entry::loadSceneFromJson(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.fail())
    {
        std::cout << "Scene file not found " << filename << std::endl;
        return;
    }

    nlohmann::json data = nlohmann::json::parse(file);

    for (auto &meshData : data["meshes"])
    {
        ModelComponent &model = scene.createModelComponent(&renderManager.atlasManager, meshData["model"], meshData["diffuseTexture"]);
        if (meshData["position"] != nullptr)
        {
            model.setPosition(getVec3FromJsonArray(meshData["position"]));
        }
        if (meshData["rotation"] != nullptr)
        {
            model.setRotation(getVec3FromJsonArray(meshData["rotation"]));
        }
        if (meshData["scale"] != nullptr)
        {
            model.setScale(getVec3FromJsonArray(meshData["scale"]));
        }
    }

    for (auto &lightData : data["lights"])
    {
        std::string type = lightData["type"];

        bool doesCastShadows = false;
        if (lightData["doesCastShadows"] != nullptr)
        {
            doesCastShadows = lightData["doesCastShadows"];
        }

        if (type == "POINT")
        {
            Light &light = scene.createPointLight(
                    getVec3FromJsonArray(lightData["position"]),
                    getVec3FromJsonArray(lightData["color"]),
                lightData["distAttenMax"],
                lightData["intensity"]);

            light.doesCastShadows = doesCastShadows;
        }
        if (type == "SPOT")
        {
            Light &light = scene.createSpotLight(
                    &renderManager.atlasManager,
                    getVec3FromJsonArray(lightData["position"]),
                    getVec3FromJsonArray(lightData["direction"]),
                    getVec3FromJsonArray(lightData["color"]),
                lightData["beamAngle"],
                lightData["distAttenMax"],
                lightData["intensity"]);

            light.doesCastShadows = doesCastShadows;
        }
    }
}

bool Entry::handleInputEvent(InputEvent *const event)
{
    if (event->type == InputEvent::KEYDOWN)
    {
        if (event->keyCode == 30) // '1'
        {
            debugMode = 0;
        }
        if (event->keyCode == 31) // '2'
        {
            debugMode = 1;
        }
        if (event->keyCode == 32) // '3'
        {
            debugMode = 2;
        }
        if (event->keyCode == 33) // '4'
        {
            debugMode = 3;
        }
//        std::cout << event->keyCode << std::endl;
    }

    return true;
}