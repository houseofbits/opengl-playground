#include "PhysicsDebugRenderSystem.h"
#include "../../Application/Events/SystemEvent.h"

PhysicsDebugRenderSystem::PhysicsDebugRenderSystem() : EntitySystem(), m_isEnabled(false), m_activeCameraHelper() {
}

void PhysicsDebugRenderSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    resourceManager.request(m_PhysicsResource, "physics");
    resourceManager.request(m_ShaderProgram, "resources/shaders/basic|.vert|.frag");

    m_wireframeRenderer.initialize();
}

void PhysicsDebugRenderSystem::process(EventManager &) {

    if (!m_isEnabled) {
        return;
    }

    Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
    if (camera == nullptr) {
        return;
    }

    m_ShaderProgram().use();
    camera->bind(m_ShaderProgram());

    m_PhysicsResource().drawDebug(m_ShaderProgram());

    m_wireframeRenderer.setCamera(*camera);
    m_PhysicsResource().drawDebugWireframe(m_wireframeRenderer);
}

void PhysicsDebugRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsDebugRenderSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &PhysicsDebugRenderSystem::handleEditorUIEvent);
    eventManager.registerEventReceiver(this, &PhysicsDebugRenderSystem::handleCameraActivationEvent);
}

void PhysicsDebugRenderSystem::handleSystemEvent(const SystemEvent &) {

}

void PhysicsDebugRenderSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_PHYSICS) {
        m_isEnabled = true;
    } else if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_SHADED
        || event.m_Type == EditorUIEvent::TOGGLE_RENDER_REFLECTIONS
        || event.m_Type == EditorUIEvent::TOGGLE_RENDER_PROBES) {
        m_isEnabled = false;
    }
}
