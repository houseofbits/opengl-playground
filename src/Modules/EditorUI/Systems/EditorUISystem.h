#pragma once

#include "../../../Core/API.h"
#include "../../../Core/Events/RawSDLEvent.h"
#include "../../../Core/Events/WindowEvent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Renderer/Components/CameraComponent.h"
#include "../../Renderer/Components/EnvironmentProbeComponent.h"
#include "../../Renderer/Components/LightComponent.h"
#include "../../Renderer/Components/StaticMeshComponent.h"
#include "../Events/EditorUIEvent.h"
#include "../UI/EditWindowUI.h"
#include "../UI/MainToolbarUI.h"
#include "../UI/MaterialEditWindowUI.h"

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleRawSDLEvent(RawSDLEvent *event);
    Camera *findActiveCamera();
    TransformComponent *getSelectedTransformComponent();
    void openMaterialEditor(ResourceHandle<MaterialResource>& handle);

    ResourceManager *m_ResourceManager;
    bool m_isImUIInitialized;
    bool m_isDemoWindowVisible;
    MainToolbarUI m_MainToolbarUI;
    EditWindowUI m_EditWindowUI;
    MaterialEditWindowUI m_MaterialEditWindowUI;
};
