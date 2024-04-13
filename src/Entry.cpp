#include "Include.h"
#include <SDL2/SDL_opengl.h>
#include "Renderer/Texture2D.h"

Entry::Entry() : window(&eventManager)
{
    animatedLightAngle = 0;
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

        depthRenderTarget.beginRender();
        depthShader.use();
        scene.renderDepth(shadowCastingLight->getCamera(), depthShader);
        depthRenderTarget.end();

        glViewport(0, 0, window.viewportWidth, window.viewportHeight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        materialShader.use();
        renderer.updateLights();
        renderer.setShaderGlobalAttributes(&materialShader);

        scene.render(camera, materialShader);

        wireframeRenderer.draw(&renderer);

        // imageRenderer.draw();

        window.doubleBuffer();

        // Animate point light
        animatedLightAngle += Time::frameTime * 3.0;
        animatedLight->position.x = cos(animatedLightAngle) * 400;
        animatedLight->position.z = sin(animatedLightAngle) * 400;
    }

    window.destroy();
}

/**
 *  Learning
 *  - Shadow map renderer
 *  - Normal maps
 *  - Specular maps
 *  - Deferred rendering
 *  - SSAO
 *  - Animations
 */
void Entry::init()
{
    materialShader.loadProgram("resources/shaders/lighting.vert", "resources/shaders/lighting.frag");
    depthShader.loadProgram("resources/shaders/lightDepth.vert", "resources/shaders/lightDepth.frag");

    wireframeRenderer.init();

    depthRenderTarget.create(RenderTarget::TARGET_DEPTH, 512, 512);

    camera.registerEventHandlers(&eventManager);

    renderer.init(&camera);
    renderer.createPointLight(glm::vec3(0, 300, 0), glm::vec3(1, 0, 0), 600, 1);
    animatedLight = renderer.createPointLight(glm::vec3(0, 50, 300), glm::vec3(1, 1, 0), 600, 2);
    shadowCastingLight = renderer.createDirectLight(glm::vec3(100, 400, 100), glm::vec3(0, -1, 0), glm::vec3(0.8, 0.8, 1), 45, 600, 2);
    shadowCastingLight->doesCastShadows = true;

    imageRenderer.init(glm::vec4(0, 0, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimage.frag");
    imageRenderer.textureId = depthRenderTarget.targetTextureId;

    scene.createModelComponent("resources/Duck/Duck.gltf", "resources/Duck/DuckCM.png")
        .setPosition(glm::vec3(-100, 0, 0));

    scene.createModelComponent("resources/Duck/Duck.gltf", "resources/Duck/DuckCM.png")
        .setPosition(glm::vec3(100, 50, 0));

    scene.createModelComponent("resources/BoxTextured.gltf", "resources/textures/concrete.jpg")
        .setPosition(glm::vec3(0, 1, 0))
        .setScale(glm::vec3(500, 10, 500));
}