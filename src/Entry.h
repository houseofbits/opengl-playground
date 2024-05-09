#pragma once
#include "Components/ModelComponent.h"
#include "Components/Scene.h"
#include "Events/EventManager.h"
#include "PostProcessRenderer/PostProcessRenderer.h"
#include "Renderer/Camera.h"
#include "Renderer/Cube.h"
#include "Renderer/Model.h"
#include "Renderer/Plane.h"
#include "Renderer/RenderManager.h"
#include "Renderer/Shader.h"
#include "Components//SceneLoader.h"
#include "Renderer/ShadowMapRenderer.h"
#include "Renderer/TextureAtlasManager.h"
#include "Window.h"
#include "WireframeRenderer/WireframeRenderer.h"
#include <string>

class InputEvent;

class Entry
{
private:
    EventManager eventManager;
    RenderManager renderManager;
    SceneLoader sceneLoader;
    Window window;
    Scene scene;

    int debugMode;

    void init();
    bool handleInputEvent(InputEvent *event);

public:
    Entry();
    void run();
};