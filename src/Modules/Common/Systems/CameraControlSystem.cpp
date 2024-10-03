#include "CameraControlSystem.h"

CameraControlSystem::CameraControlSystem() : EntitySystem() {
    usesComponent<CameraComponent>();
}

void CameraControlSystem::process() {
}

void CameraControlSystem::initialize(ResourceManager *) {
}

void CameraControlSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraControlSystem::handleWindowEvent);
}

bool CameraControlSystem::handleWindowEvent(WindowEvent *const event) {
    Camera *camera = findActiveCamera();
    if (!camera) {
        return true;
    }

    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        camera->setViewportSize(event->window->viewportWidth, event->window->viewportHeight);
    }

    return true;
}

Camera *CameraControlSystem::findActiveCamera() {
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr && c->m_type == CameraComponent::TYPE_FREE) {
        return &c->m_Camera;
    }

    return nullptr;
}

