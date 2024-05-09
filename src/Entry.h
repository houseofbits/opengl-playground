#pragma once
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Cube.h"
#include "Renderer/Plane.h"
#include "Renderer/Camera.h"
#include "Events/EventManager.h"
#include "Renderer/Model.h"
#include "Renderer/Scene.h"
#include "Renderer/ShadowMapRenderer.h"
#include "WireframeRenderer/WireframeRenderer.h"
#include "PostProcessRenderer/PostProcessRenderer.h"
#include "Components/ModelComponent.h"
#include <string>
#include "Renderer/TextureAtlasManager.h"
#include "Renderer/RenderManager.h"

class InputEvent;

class Entry
{
private:
    EventManager eventManager;
    RenderManager renderManager;
    Window window;
    Scene scene;

    int debugMode;

    void init();
    void loadSceneFromJson(const std::string&);
    bool handleInputEvent(InputEvent *event);

public:
    Entry();
    void run();
};