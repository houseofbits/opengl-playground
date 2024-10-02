#include "CameraMouseLookBehaviourSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../../Core/Helper/Time.h"

CameraMouseLookBehaviourSystem::CameraMouseLookBehaviourSystem() : EntitySystem() {
    usesComponent<CameraComponent>();
}

void CameraMouseLookBehaviourSystem::process() {

}

void CameraMouseLookBehaviourSystem::initialize(ResourceManager *) {

}

void CameraMouseLookBehaviourSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraMouseLookBehaviourSystem::handleInputEvent);
}

bool CameraMouseLookBehaviourSystem::handleInputEvent(InputEvent *event) {
    auto *camera = findActiveCamera();
    if (!camera) {
        return true;
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        float lookSpeed = 0.15;
        camera->rotateView(-event->mouseMotion * lookSpeed * Time::frameTime);
    }

    return true;
}

CameraComponent *CameraMouseLookBehaviourSystem::findActiveCamera() {
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr && c->m_type == CameraComponent::TYPE_FREE) {
        return c;
    }

    return nullptr;
}