#include "Include.h"
#include <SDL2/SDL_opengl.h>
#include "Renderer/Texture2D.h"
#include "../libs/tinygltf/json.hpp"
#include <fstream>
#include "Events/InputEvent.h"

Entry::Entry() : window(&eventManager)
{
    animatedLightAngle = 0;
    isShadowAtlasVisible = false;
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

        renderer.updateLights(scene);

        renderer.renderShadowAtlas(scene);

        glViewport(0, 0, window.viewportWidth, window.viewportHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        (*scene.lights.begin())->position = camera.getPosition() - glm::vec3(0, 20, 0);
        (*scene.lights.begin())->direction = camera.getDirection();

        materialShader.use();
        renderer.setShaderAttributes(materialShader);
        scene.render(camera, materialShader);

        // wireframeRenderer.draw(&renderer);

        if (isShadowAtlasVisible)
        {
            renderer.shadowMapRenderer.debugRender();
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
 *  - Add texture atlas mapping uniform
 *  - Add point lights
 *  - Add direct lights
 *  - Add projectors
 *
 *  Implement
 *  - Point light shadow maps
 *  - Shadow maps for multiple lights
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
    wireframeRenderer.init();

    depthRenderTarget.create(RenderTarget::TARGET_DEPTH, 128, 128);
    testColorRenderTarget.create(RenderTarget::TARGET_COLOR, 128 * 6, 128);

    camera.registerEventHandlers(&eventManager);

    // shadowMapRenderer.init();
    renderer.init(&camera);

    // loadSceneFromJson("resources/scenes/ducks-n-lights.json");
    // loadSceneFromJson("resources/scenes/multiple-spot-lights.json");
    // loadSceneFromJson("resources/scenes/single-spot-light.json");
    loadSceneFromJson("resources/scenes/hall-with-columns.json");
}

glm::vec3 getVec3FromnJsonArray(nlohmann::json::array_t array)
{
    glm::vec3 value(0.0);

    value.x = array[0];
    value.y = array[1];
    value.z = array[2];

    return value;
}

void Entry::loadSceneFromJson(std::string filename)
{
    std::ifstream file(filename);
    if (file.fail())
    {
        std::cout << "Scene file not found " << filename << std::endl;
        return;
    }

    nlohmann::json data = nlohmann::json::parse(file);

    materialShader.loadProgram(data["materialShaderVert"], data["materialShaderFrag"]);

    for (auto &meshData : data["meshes"])
    {
        ModelComponent &model = scene.createModelComponent(meshData["model"], meshData["diffuseTexture"]);
        if (meshData["position"] != nullptr)
        {
            model.setPosition(getVec3FromnJsonArray(meshData["position"]));
        }
        if (meshData["rotation"] != nullptr)
        {
            model.setRotation(getVec3FromnJsonArray(meshData["rotation"]));
        }
        if (meshData["scale"] != nullptr)
        {
            model.setScale(getVec3FromnJsonArray(meshData["scale"]));
        }
    }

    for (auto &lightData : data["lights"])
    {
        std::string type = lightData["type"];
        Light *light = nullptr;
        if (type == "POINT")
        {
            light = scene.createPointLight(
                getVec3FromnJsonArray(lightData["position"]),
                getVec3FromnJsonArray(lightData["color"]),
                lightData["distAttenMax"],
                lightData["intensity"]);

            animatedLight = light;
        }
        if (type == "SPOT")
        {
            light = scene.createDirectLight(
                getVec3FromnJsonArray(lightData["position"]),
                getVec3FromnJsonArray(lightData["direction"]),
                getVec3FromnJsonArray(lightData["color"]),
                lightData["beamAngle"],
                lightData["distAttenMax"],
                lightData["intensity"]);
        }
        if (light != nullptr && lightData["doesCastShadows"] != nullptr)
        {
            light->doesCastShadows = lightData["doesCastShadows"];
        }
    }
}

bool Entry::handleInputEvent(InputEvent *const event)
{
    if (event->type == InputEvent::KEYDOWN)
    {
        if (event->keyCode == 30) // '1'
        {
            testFramebuffer = 1;
        }
        if (event->keyCode == 31) // '2'
        {
            testFramebuffer = 2;
        }
        if (event->keyCode == 59) // F2
        {
            isShadowAtlasVisible = !isShadowAtlasVisible;
        }
        // std::cout << event->keyCode << std::endl;
    }

    return true;
}