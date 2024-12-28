#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/RawSDLEvent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Renderer/Components/EnvironmentProbeComponent.h"
#include "../../Renderer/Components/LightComponent.h"
#include "../../Renderer/Components/StaticMeshComponent.h"
#include "../Events/EditorUIEvent.h"
#include "../UI/EditWindowUI.h"
#include "../UI/MainToolbarUI.h"
#include "../UI/MaterialEditWindowUI.h"
#include "../Helpers/TransformGizmoHelper.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleRawSDLEvent(const RawSDLEvent &);

    TransformComponent *getSelectedTransformComponent();

    void openMaterialEditor(ResourceHandle<MaterialResource> &handle);

    template <class ComponentT, class EditorT>
    void registerComponentEditor() {
        m_componentEditors[ComponentT::TypeName()] = new EditorT();
    }

    ResourceManager *m_ResourceManager;
    bool m_isImUIInitialized;
    TransformGizmoHelper m_transformGizmo;
    MainToolbarUI m_MainToolbarUI;
    EditWindowUI m_EditWindowUI;
    MaterialEditWindowUI m_MaterialEditWindowUI;
    std::map<std::string, BaseComponentEdit*> m_componentEditors;
    ActiveCameraHelper m_activeCameraHelper;
    SingleComponentRegistry<EditorCameraComponent>* m_editorCameraComponentRegistry;
    SingleComponentRegistry<CameraComponent>* m_cameraComponentRegistry;
    Identity::Type m_selectedEntityId;

private:
    void processDockSpaceWindow();
};
