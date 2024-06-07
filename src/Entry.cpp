#include "Events/InputEvent.h"
#include "Include.h"
#include "Renderer/Texture/Texture2D.h"

Entry::Entry() : window(&eventManager), renderManager(&window), debugMode(0), sceneLoader(&renderManager), cameraController()
{
    eventManager.registerEventReceiver(this, &Entry::handleInputEvent);
    cameraController.registerEventHandlers(&eventManager);
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

    renderManager.preRender(scene);

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
 * TODO: BLOCKING
*  - Add lighting to test scene
 *
 * TODO: NICE TO HAVE
 *  - Material structure
 *  - Serialization/deserialization of objects
 *  - Tweak mip-mapping algorithm
 *  - Add shadow bias variables to light
 *  - Projection texture per light view
 *  - Optional light views for omni lights - configurable
 *  - Add View abstraction - could be used for Camera, LightView, Decals, RTT
 *
 * TODO: EXPLORE
 *  - Basic PhysX - MVP: have static scene collision geometry and player controller
 *  - Explore animation system
 *  - Post process render effects (SSAO, SSR, HDR & Gamma correction)
 *
 * Instanced rendering for all the static and semi-static geometry
 *  Materials array - shader storage buffer initiated at the load time
 *  Instance data array - having materialId and modelMatrix
 *
 */
void Entry::init()
{
    renderManager.init();

//     sceneLoader.loadSceneFromJson("resources/scenes/hall-with-columns-omni.json", scene);
     sceneLoader.loadSceneFromJson("resources/scenes/scene1.json", scene);
//     sceneLoader.loadSceneFromJson("resources/scenes/gltf-debug.json", scene);

//     scene.camera.registerEventHandlers(&eventManager);
     cameraController.attachCamera(&scene.camera);
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
        if (event->keyCode == 15) // 'L'
        {
            renderManager.isLightConesVisible = !renderManager.isLightConesVisible;
        }
//
//        std::cout << event->keyCode << std::endl;
    }

    return true;
}