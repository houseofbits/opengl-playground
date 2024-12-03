#pragma once

#include "../../../Core/API.h"
#include "../../../Core/Events/RawSDLEvent.h"
#include "../../../Core/Events/WindowEvent.h"
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

class EditorUISystem : public EntitySystem {
public:
    EditorUISystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleWindowEvent(const WindowEvent &);

    void handleRawSDLEvent(const RawSDLEvent &);

    Camera *findActiveCamera();

    TransformComponent *getSelectedTransformComponent();

    void openMaterialEditor(ResourceHandle<MaterialResource> &handle);

    template <class ComponentT, class EditorT>
    void registerComponentEditor() {
        m_componentEditors[ComponentT::TypeName()] = new EditorT();
    }

    ResourceManager *m_ResourceManager;
    bool m_isImUIInitialized;
    bool m_isDemoWindowVisible;
    TransformGizmoHelper m_transformGizmo;
    MainToolbarUI m_MainToolbarUI;
    EditWindowUI m_EditWindowUI;
    MaterialEditWindowUI m_MaterialEditWindowUI;
    std::map<std::string, BaseComponentEdit*> m_componentEditors;

private:
    void processDockSpaceWindow();
};
