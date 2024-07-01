#pragma once

#include "../../../CoreV2/API.h"
#include "../../../CoreV2/Events/RawSDLEvent.h"
#include "../../../CoreV2/Events/WindowEvent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Renderer/Components/CameraComponent.h"
#include "../../Renderer/Components/LightComponent.h"
#include "../../Renderer/Components/StaticMeshComponent.h"
#include "../Events/EditorUIEvent.h"
#include "../UI/EditWindowUI.h"
#include "../UI/MainToolbarUI.h"

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void registerComponent(Component *comp) override;
    void unregisterComponent(Component *comp) override;
    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleRawSDLEvent(RawSDLEvent *event);

    Camera *findActiveCamera();
    TransformComponent *getSelectedTransformComponent();

    ResourceManager *m_ResourceManager;
    bool m_isImUIInitialized;
    bool m_isDemoWindowVisible;

    std::map<Identity::Type, LightComponent *> m_lightComponents;
    std::map<Identity::Type, StaticMeshComponent *> m_meshComponents;
    std::map<Identity::Type, TransformComponent *> m_transformComponents;
    std::map<Identity::Type, CameraComponent *> m_cameraComponents;

    MainToolbarUI m_MainToolbarUI;
    EditWindowUI m_EditWindowUI;
};
