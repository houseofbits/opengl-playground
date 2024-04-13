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

class Entry
{
private:
    EventManager eventManager;
    Window window;
    Renderer renderer;
    WireframeRenderer wireframeRenderer;
    PostProcessRenderer imageRenderer;
    RenderTarget depthRenderTarget;

    Scene scene;
    Camera camera;
    Shader materialShader;
    Shader depthShader;

    // Rotating point light
    Light *animatedLight;
    float animatedLightAngle;
    Light *shadowCastingLight;

    void init();

public:
    Entry();
    void run();
};