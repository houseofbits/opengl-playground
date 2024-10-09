#include "CharacterPickingBehaviour.h"
#include "../Helpers/PhysicsRayCastResult.h"
#include "../Components/CharacterControllerComponent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Events/PhysicsPickingEvent.h"


CharacterPickingBehaviour::CharacterPickingBehaviour() : EntityBehaviour() {

}

void CharacterPickingBehaviour::initialize(ResourceManager &) {

}

void CharacterPickingBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterPickingBehaviour::handleInputEvent);
}

void CharacterPickingBehaviour::handleInputEvent(const InputEvent *const event) {
    if ((event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) ||
        (event->type == InputEvent::MOUSEUP && event->mouseButtonRight)) {

        auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
        if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
            return;
        }

        auto *characterComponent = m_Entity->getComponent<CharacterControllerComponent>();
        if (characterComponent == nullptr) {
            return;
        }

        PhysicsRayCastResult hit;
        if (characterComponent->rayCast(cameraComponent->m_Camera.position,
                                        cameraComponent->m_Camera.getViewDirection() * 10.f, hit)) {
            auto evt = m_EventManager->createEvent<PhysicsPickingEvent>();
            evt->m_entityId = hit.m_entityId;
            evt->m_distance = hit.m_distance;
            evt->m_touchPoint = hit.m_touchPoint;
            evt->m_doActivate = event->mouseButtonRight;
            m_EventManager->queueEvent(evt);
        }

    }
}

std::string CharacterPickingBehaviour::getDescription() {
    return "Casts a ray from character viewpoint and emits picking event";
}
