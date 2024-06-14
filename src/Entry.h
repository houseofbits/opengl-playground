#pragma once
#include "Components/ModelComponent.h"
#include "Components/Scene.h"
#include "Events/EventManager.h"
#include "Physics/TestPhysics.h"
#include "PostProcessRenderer/PostProcessRenderer.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Camera/CameraController.h"
#include "Renderer/Model/Cube.h"
#include "Renderer/Model/Model.h"
#include "Renderer/Model/Plane.h"
#include "Renderer/RenderManager.h"
#include "Renderer/Renderer/ShadowMapRenderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/TextureAtlas/TextureAtlasManager.h"
#include "SceneLoader/SceneLoader.h"
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
    CameraController cameraController;

    TestPhysics physics;

    int debugMode;

    void init();
    bool handleInputEvent(InputEvent *event);

public:
    Entry();
    void run();
};