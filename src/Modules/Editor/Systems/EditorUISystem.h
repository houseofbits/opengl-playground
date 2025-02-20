#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/RawSDLEvent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../UI/EditWindowUI.h"
#include "../UI/MainToolbarUI.h"
#include "../UI/MaterialEditWindowUI.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"
#include "../Components/EditorCameraComponent.h"
#include "../Helpers/TransformHelper.h"
#include "../../../Renderer/Shader/WireframeRenderer.h"
#include "../../../Renderer/Model/WireCube.h"
#include "../../../Renderer/Model/WireSphere.h"

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleRawSDLEvent(const RawSDLEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

    TransformComponent *getSelectedTransformComponent();

    void openMaterialEditor(ResourceHandle<MaterialResource> &handle);

    template<class ComponentT, class EditorT>
    void registerComponentEditor() {
        m_componentEditors[ComponentT::TypeName()] = new EditorT(*this);
    }

    void selectEntity(Entity &);

    void clearEntitySelection();

    Entity* getSelectedEntity() const;

    ResourceManager *m_ResourceManager;
    bool m_isImUIInitialized;
    // TransformGizmoHelper m_transformGizmo;
    MainToolbarUI m_MainToolbarUI;
    EditWindowUI m_EditWindowUI;
    MaterialEditWindowUI m_MaterialEditWindowUI;
    std::map<std::string, BaseComponentEdit *> m_componentEditors;
    ActiveCameraHelper m_activeCameraHelper;
    EntityUniqueComponentRegistry<EditorCameraComponent> *m_editorCameraComponentRegistry;
    EntityUniqueComponentRegistry<CameraComponent> *m_cameraComponentRegistry;
    bool m_isEditorModeEnabled;

    TransformHelper m_transformHelper;

private:
    void processDockSpaceWindow();

    Identity::Type m_selectedEntityId;

    WireframeRenderer m_wireframeRenderer;
    WireCube wfcube;
    WireSphere wfsphere;
};
