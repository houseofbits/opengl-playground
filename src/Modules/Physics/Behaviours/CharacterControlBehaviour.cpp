#include "CharacterControlBehaviour.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/CharacterControllerComponent.h"

CharacterControlBehaviour::CharacterControlBehaviour() : EntityBehaviour() {

}

void CharacterControlBehaviour::initialize(ResourceManager &) {

}

void CharacterControlBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterControlBehaviour::handleInputEvent);
}

std::string CharacterControlBehaviour::getDescription() {
    return "Controls movement of the character (WASD)";
}

void CharacterControlBehaviour::handleInputEvent(const InputEvent *const event) {
    if (event->type != InputEvent::KEYPRESS) {
        return;
    }

    auto* characterComponent = m_Entity->getComponent<CharacterControllerComponent>();
    if (characterComponent == nullptr) {
        return;
    }

    auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
    if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
        return;
    }

    glm::vec3 forwardDirection = cameraComponent->m_Camera.getViewDirection();
    forwardDirection.y = 0;
    forwardDirection = glm::normalize(forwardDirection);
    glm::vec3 rightDirection = glm::normalize(glm::cross(forwardDirection, glm::vec3(0, 1, 0)));
    bool doMove = false;
    glm::vec3 movementDirection(0);

    //W
    if (event->keyCode == 26) {
        doMove = true;
        movementDirection = movementDirection + forwardDirection;
    }
    //S
    if (event->keyCode == 22) {
        doMove = true;
        movementDirection = movementDirection - forwardDirection;
    }
    //A
    if (event->keyCode == 4) {
        doMove = true;
        movementDirection = movementDirection - rightDirection;
    }
    //D
    if (event->keyCode == 7) {
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
