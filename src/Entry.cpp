#include "Include.h"
#include "Renderer/Texture2D.h"
#include "Events/InputEvent.h"

Entry::Entry() : window(&eventManager), renderManager(&window), debugMode(0), sceneLoader(&renderManager)
{
    eventManager.registerEventReceiver(this, &Entry::handleInputEvent);
}

void Entry::run()
{
    window.create();

    init();

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

//        (*scene.lights.begin())->position = scene.camera.getPosition() - glm::vec3(0, 0.1, 0);
//        (*scene.lights.begin())->direction = scene.camera.getDirection();

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
 *  - Render scene to cubemap
 *  - Reflection probe manager and renderer
 *  - Refactor file structure
 *  - Debug point lights - has some rendering glitches
 *  - Explore how to render debug information - wireframe generation for lights
 *
 * TODO: NICE TO HAVE
 *  - Add shadow bias variables to light
 *  - Projection texture per light view
 *  - Optional light views for omni lights - configurable
 *  - Add View abstraction - could be used for Camera, LightView, Decals, RTT
 *
 * TODO: IMPLEMENT
 *  - Create a basic test scene
 *  - Basic animations
 *  - PhysX
 *
 */
void Entry::init()
{
    renderManager.init();

//     loadSceneFromJson("resources/scenes/ducks-n-lights.json");
//     loadSceneFromJson("resources/scenes/multiple-spot-lights.json");
//    loadSceneFromJson("resources/scenes/single-spot-light.json");
//     loadSceneFromJson("resources/scenes/hall-with-columns.json");
//     loadSceneFromJson("resources/scenes/hall-with-columns-omni.json");
//     sceneLoader.loadSceneFromJson("resources/scenes/hall-with-columns-omni.json", scene);
     sceneLoader.loadSceneFromJson("resources/scenes/scene1.json", scene);
//     sceneLoader.loadSceneFromJson("resources/scenes/gltf-debug.json", scene);
//     sceneLoader.loadSceneFromJson("resources/scenes/normal-maps.json", scene);

     scene.camera.registerEventHandlers(&eventManager);
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
        if (event->keyCode == 34) // '5'
        {
            debugMode = 4;
        }
//        std::cout << event->keyCode << std::endl;
    }

    return true;
}