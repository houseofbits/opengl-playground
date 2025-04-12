#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../../../Renderer/Shader/WireframeRenderer.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"

class EditorPostRender : public EntitySystem {
public:
    EditorPostRender();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

private:
    void renderPhysicsMesh(glm::mat4& parentTransform, const PhysicsMeshResource& mesh);

    EntityUniqueComponentRegistry<PhysicsBodyComponent> *m_physicsBodiesRegistry;
    bool m_isEditorModeEnabled{};
    bool m_isImUIInitialized{};
    WireframeRenderer m_wireframeRenderer;
    ActiveCameraHelper m_activeCameraHelper;
};
