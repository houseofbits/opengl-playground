#include "MainCharacterBehaviourSystem.h"
#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../../Core/Helper/Time.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"

MainCharacterBehaviourSystem::MainCharacterBehaviourSystem() : EntitySystem() {
    usesComponent<MainCharacterBehaviourComponent>();
}

void MainCharacterBehaviourSystem::initialize(ResourceManager &) {

}

void MainCharacterBehaviourSystem::process(EventManager &) {

}

void MainCharacterBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MainCharacterBehaviourSystem::handleInputEvent);
}

void MainCharacterBehaviourSystem::handleInputEvent(const InputEvent &event) {

    auto entity = getCharacterEntity();
    if (!entity) {
        return;
    }
    auto cameraComponent = entity->getComponent<CameraComponent>();
    if (!cameraComponent || !cameraComponent->m_isActive) {
        return;
    }
    auto *characterComponent = entity->getComponent<PhysicsCharacterComponent>();
    if (characterComponent == nullptr) {
        return;
    }

    handleMouseLook(event, characterComponent, cameraComponent);
    handleMovement(event, characterComponent, cameraComponent);
    handleAction(event, characterComponent, cameraComponent);
}

void MainCharacterBehaviourSystem::handleMouseLook(const InputEvent &event,
                                                   PhysicsCharacterComponent *characterComponent,
                                                   CameraComponent *cameraComponent) {
    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event.mouseMotion * lookSpeed * Time::frameTime);

        characterComponent->setLookingDirection(cameraComponent->m_Camera.getViewDirection());
    }
}

void MainCharacterBehaviourSystem::handleMovement(const InputEvent &event,
                                                  PhysicsCharacterComponent *characterComponent,
                                                  CameraComponent *cameraComponent) {
    if (event.type == InputEvent::KEYPRESS) {
        glm::vec3 forwardDirection = cameraComponent->m_Camera.getViewDirection();
        forwardDirection.y = 0;
        forwardDirection = glm::normalize(forwardDirection);
        glm::vec3 rightDirection = glm::normalize(glm::cross(forwardDirection, glm::vec3(0, 1, 0)));
        bool doMove = false;
        glm::vec3 movementDirection(0);

        //W
        if (event.keyCode == 26) {
            doMove = true;
            movementDirection = movementDirection + forwardDirection;
        }
        //S
        if (event.keyCode == 22) {
            doMove = true;
            movementDirection = movementDirection - forwardDirection;
        }
        //A
        if (event.keyCode == 4) {
            doMove = true;
            movementDirection = movementDirection - rightDirection;
        }
        //D
        if (event.keyCode == 7) {
            doMove = true;
            movementDirection = movementDirection + rightDirection;
        }
        //Space-bar
//    if (event->type == InputEvent::KEYPRESS && event->keyCode == 44) {
//        doMove = true;
//    }

        if (doMove) {
            characterComponent->move(glm::normalize(movementDirection));
        }
    }
}

void MainCharacterBehaviourSystem::handleAction(const InputEvent &event,
                                                PhysicsCharacterComponent *characterComponent,
                                                CameraComponent *cameraComponent) {
    if ((event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) ||
        (event.type == InputEvent::MOUSEUP && event.mouseButtonRight)) {

        PhysicsRayCastResult hit;
        if (characterComponent->rayCast(cameraComponent->m_Camera.position,
                                        cameraComponent->m_Camera.getViewDirection() * 10.f, hit)) {

            event.m_EventManager->queueEvent<PhysicsPickingEvent>(
                    hit.m_entityId,
                    hit.m_distance,
                    hit.m_touchPoint,
                    event.mouseButtonRight
            );
        }
    }
}
