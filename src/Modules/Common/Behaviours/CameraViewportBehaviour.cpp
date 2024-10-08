#include "CameraViewportBehaviour.h"
#include "../Components/CameraComponent.h"

CameraViewportBehaviour::CameraViewportBehaviour() : EntityBehaviour() {

}

void CameraViewportBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CameraViewportBehaviour::handleWindowEvent);
}

std::string CameraViewportBehaviour::getDescription() {
    return "Resizes camera viewport on window resize";
}

void CameraViewportBehaviour::handleWindowEvent(const WindowEvent *const event) {
    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
        if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
            return;
        }

        cameraComponent->m_Camera.setViewportSize(event->window->viewportWidth, event->window->viewportHeight);
    }
}
