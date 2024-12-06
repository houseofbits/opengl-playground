#include "PhysicsDebugRenderSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Editor/Components/EditorCameraComponent.h"

PhysicsDebugRenderSystem::PhysicsDebugRenderSystem() : EntitySystem(), m_isEnabled(false), m_activeCameraId(0) {
}

void PhysicsDebugRenderSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
    resourceManager.request(m_ShaderProgram, "resources/shaders/basic|.vert|.frag");
}

void PhysicsDebugRenderSystem::process(EventManager &) {

    if (!m_isEnabled) {
        return;
    }

    Camera *camera = findActiveCamera();
    if (camera == nullptr) {
        return;
    }

    m_ShaderProgram().use();
    camera->bind(m_ShaderProgram());

    m_PhysicsResource().drawDebug(m_ShaderProgram());
}

void PhysicsDebugRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsDebugRenderSystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &PhysicsDebugRenderSystem::handleEditorUIEvent);
}

void PhysicsDebugRenderSystem::handleWindowEvent(const WindowEvent &) {

}

void PhysicsDebugRenderSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    m_isEnabled = false;
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_PHYSICS) {
        m_isEnabled = true;
    }
}

Camera *PhysicsDebugRenderSystem::findActiveCamera() {
    if (!m_activeCameraId) {
        return nullptr;
    }

    auto entity = m_EntityContext->getEntity(m_activeCameraId);

    if (!entity) {
        return nullptr;
    }

    auto camera = entity->getComponent<BaseCameraComponent>();
    if (camera != nullptr) {
        return &camera->m_Camera;
    }
//
//    auto editorCamera = entity->getComponent<EditorCameraComponent>();
//    if (editorCamera != nullptr) {
//        return &editorCamera->m_Camera;
//    }

    return nullptr;
}

void PhysicsDebugRenderSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    m_activeCameraId = event.m_cameraComponentId;
}
