#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../../../Renderer/Shader/WireframeRenderer.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"
#include "../Events//EditorUIEvent.h"

class EditorPostRender : public EntitySystem {
public:
    EditorPostRender();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &event);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

private:
    EntityUniqueComponentRegistry<PhysicsBodyComponent> *m_physicsBodiesRegistry;
    bool m_isEditorModeEnabled{};
    bool m_isImUIInitialized{};
    bool m_doREnderWireframes{true};
    WireframeRenderer m_wireframeRenderer;
    ActiveCameraHelper m_activeCameraHelper;
};
