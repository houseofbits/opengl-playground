#include "EditorCameraSystem.h"
#include "../Components/EditorCameraComponent.h"

EditorCameraSystem::EditorCameraSystem() : EntitySystem(), m_viewportSize(0, 0) {
    usesComponent<EditorCameraComponent>();
}

void EditorCameraSystem::process(EventManager &) {

}

void EditorCameraSystem::initialize(ResourceManager &) {

}

void EditorCameraSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleCameraActivationEvent);
}

void EditorCameraSystem::handleInputEvent(const InputEvent &) {

}

void EditorCameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto [id, component]: getComponentContainer<EditorCameraComponent>()) {
        component->setActive(false);

        if (id == event.m_cameraComponentId) {
            component->setActive(true);
            component->setViewportSize(m_viewportSize);
        }
    }
}

void EditorCameraSystem::handleWindowEvent(const WindowEvent &event) {
    if (event.eventType == WindowEvent::RESIZE || event.eventType == WindowEvent::CREATE) {
        m_viewportSize.x = event.window->viewportWidth;
        m_viewportSize.y = event.window->viewportHeight;
    }
}