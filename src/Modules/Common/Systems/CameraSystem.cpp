#include "CameraSystem.h"
#include "../Components/CameraComponent.h"

CameraSystem::CameraSystem() {
    usesComponent<CameraComponent>();
}

void CameraSystem::process(EventManager &) {

}

void CameraSystem::initialize(ResourceManager &) {

}

void CameraSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CameraSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &CameraSystem::handleCameraActivationEvent);
}

void CameraSystem::handleInputEvent(const InputEvent &) {

}

void CameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto comp: getComponentContainer<CameraComponent>()) {
        comp.second->m_isActive = false;

        if (comp.first == event.m_cameraComponentId) {
            comp.second->m_isActive = true;
        }
    }
}
