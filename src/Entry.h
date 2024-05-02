#pragma once
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Cube.h"
#include "Renderer/Plane.h"
#include "Renderer/Camera.h"
#include "Events/EventManager.h"
#include "Renderer/Model.h"
#include "Renderer/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/ShadowMapRenderer.h"
#include "WireframeRenderer/WireframeRenderer.h"
#include "PostProcessRenderer/PostProcessRenderer.h"
#include "Components/ModelComponent.h"
#include <string>
#include "Renderer/TextureAtlasManager.h"

class InputEvent;

class Entry
{
private:
    EventManager eventManager;
    Window window;
    Renderer renderer;
    WireframeRenderer wireframeRenderer;

    Scene scene;
    Camera camera;
    Shader materialShader;
    ShadowMapRenderer shadowMapRenderer;

    //Test shadow atlases
    TextureAtlasManager atlasManager;
    PostProcessRenderer postProcessRenderer;


    // Rotating point light
    Light *animatedLight;
    float animatedLightAngle;
    int testFramebuffer;

    bool isShadowAtlasVisible;

    void init();
    void loadSceneFromJson(std::string);

    bool handleInputEvent(InputEvent *const event);

public:
    Entry();
    void run();
};