#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Physics/Resources/PhysicsResource.h"
#include "../Resources/ShaderProgramResource.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"

class PhysicsDebugRenderSystem : public EntitySystem {
public:
    PhysicsDebugRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

private:

    bool m_isEnabled;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ActiveCameraHelper m_activeCameraHelper;
};
