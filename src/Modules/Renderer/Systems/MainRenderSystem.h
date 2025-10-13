#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"

class RenderQueueResource;
class DeferredRenderTargetResource;

class MainRenderSystem : public EntitySystem {
public:
    MainRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

private:
    bool m_isEnabled;
    int m_viewportWidth;
    int m_viewportHeight;

    ActiveCameraHelper m_activeCameraHelper;
    ResourceHandle<RenderQueueResource> m_mainRenderQueue;
};
