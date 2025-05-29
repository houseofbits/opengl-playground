#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Physics/Resources/PhysicsResource.h"
#include "../Resources/ShaderProgramResource.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../../Renderer/Shader/WireframeRenderer.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"

class PhysicsDebugRenderSystem : public EntitySystem {
public:
    PhysicsDebugRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

private:

    bool m_isEnabled;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ActiveCameraHelper m_activeCameraHelper;
    WireframeRenderer m_wireframeRenderer;
};
