#pragma once

#include "../../../CoreV2/API.h"
#include "../../../CoreV2/Events/RawSDLEvent.h"
#include "../../../CoreV2/Events/WindowEvent.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Renderer/Components/CameraComponent.h"
#include "../../Renderer/Components/LightComponent.h"

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void registerComponent(Component *comp) override;
    void unregisterComponent(Component *comp) override;
    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleRawSDLEvent(RawSDLEvent *event);

    Camera* findActiveCamera();
    void processToolbar();
    void processLightsWindow();
    void processTransformComponent();

    bool m_isImUIInitialized;
    bool m_isDemoWindowVisible;

    bool m_isLightsWindowVisible;

    std::map<Identity::Type, LightComponent *> m_lightComponents;
    std::map<Identity::Type, TransformComponent *> m_transformComponents;
    std::map<Identity::Type, CameraComponent *> m_cameraComponents;

    int m_selectedLightEntity;
    long m_currentGizmoOperation;
    long m_currentGizmoMode;
};
