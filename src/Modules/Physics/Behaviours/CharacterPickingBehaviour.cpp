#include "CharacterPickingBehaviour.h"
#include "../Helpers/PhysicsRayCastResult.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Events/PhysicsPickingEvent.h"


CharacterPickingBehaviour::CharacterPickingBehaviour() : EntityBehaviour() {

}

void CharacterPickingBehaviour::initialize(ResourceManager &) {

}

void CharacterPickingBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterPickingBehaviour::handleInputEvent);
}

void CharacterPickingBehaviour::handleInputEvent(const InputEvent &event) {
    if ((event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) ||
        (event.type == InputEvent::MOUSEUP && event.mouseButtonRight)) {

        auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
        if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
            return;
        }

        auto *characterComponent = m_Entity->getComponent<PhysicsCharacterComponent>();
        if (characterComponent == nullptr) {
            return;
        }

        PhysicsRayCastResult hit;
        if (characterComponent->rayCast(cameraComponent->m_Camera.position,
                                        cameraComponent->m_Camera.getViewDirection() * 10.f, hit)) {
            m_EventManager->queueEvent<PhysicsPickingEvent>(
                    hit.m_entityId,
                    hit.m_distance,
                    hit.m_touchPoint,
                    event.mouseButtonRight
            );
        }

    }
}

std::string CharacterPickingBehaviour::getDescription() {
    return "Casts a ray from character viewpoint and emits picking event";
}
