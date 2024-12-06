#include "EditorCameraSystem.h"
#include "../Components/EditorCameraComponent.h"

EditorCameraSystem::EditorCameraSystem() {
    usesComponent<EditorCameraComponent>();
}

void EditorCameraSystem::process(EventManager &) {

}

void EditorCameraSystem::initialize(ResourceManager &) {

}

void EditorCameraSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleCameraActivationEvent);
}

void EditorCameraSystem::handleInputEvent(const InputEvent &) {

}

void EditorCameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto comp: getComponentContainer<EditorCameraComponent>()) {
        comp.second->m_isActive = false;

        if (comp.first == event.m_cameraComponentId) {
            comp.second->m_isActive = true;
        }
    }
}
