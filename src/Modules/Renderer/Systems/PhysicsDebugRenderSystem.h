#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Physics/Resources/PhysicsResource.h"
#include "../Resources/ShaderProgramResource.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Events/CameraActivationEvent.h"

class PhysicsDebugRenderSystem : public EntitySystem {
public:
    PhysicsDebugRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleWindowEvent(const WindowEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &);

private:
    Camera *findActiveCamera();

    bool m_isEnabled;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    Identity::Type m_activeCameraId;
};
