#include "PhysicsDebugRenderSystem.h"
#include "../../Common/Components/CameraComponent.h"


PhysicsDebugRenderSystem::PhysicsDebugRenderSystem() : EntitySystem(), m_isEnabled(false), m_renderer(nullptr) {
    usesComponent<CameraComponent>();
}

void PhysicsDebugRenderSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");

    resourceManager.request(m_ShaderProgram, "resources/shaders/basic|.vert|.frag");

    m_renderer = PhysicsDebugRenderer::getInstance();
    m_renderer->m_shader = &m_ShaderProgram.get();
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

    m_PhysicsResource().drawDebug(*m_renderer);
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
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr) {
        return &c->m_Camera;
    }

    return nullptr;
}
