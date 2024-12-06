#include "CameraSystem.h"
#include "../Components/CameraComponent.h"

CameraSystem::CameraSystem() : EntitySystem(), m_viewportSize(0, 0) {
    usesComponent<CameraComponent>();
}

void CameraSystem::process(EventManager &) {

}

void CameraSystem::initialize(ResourceManager &) {

}

void CameraSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CameraSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &CameraSystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &CameraSystem::handleCameraActivationEvent);
}

void CameraSystem::handleInputEvent(const InputEvent &) {

}

void CameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto [id, component]: getComponentContainer<CameraComponent>()) {
        component->setActive(false);

        if (id == event.m_cameraComponentId) {
            component->setActive(true);
            component->setViewportSize(m_viewportSize);
        }
    }
}

void CameraSystem::handleWindowEvent(const WindowEvent &event) {
    if (event.eventType == WindowEvent::RESIZE || event.eventType == WindowEvent::CREATE) {
        m_viewportSize.x = event.window->viewportWidth;
        m_viewportSize.y = event.window->viewportHeight;
    }
}
