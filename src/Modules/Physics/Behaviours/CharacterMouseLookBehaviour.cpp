#include "CharacterMouseLookBehaviour.h"
#include "../../../Core/Helper/Time.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/CharacterControllerComponent.h"

CharacterMouseLookBehaviour::CharacterMouseLookBehaviour() : EntityBehaviour() {

}

void CharacterMouseLookBehaviour::initialize(ResourceManager &) {

}

void CharacterMouseLookBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterMouseLookBehaviour::handleInputEvent);
}

void CharacterMouseLookBehaviour::handleInputEvent(const InputEvent & event) {
    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
        auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
        if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
            return;
        }

        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event.mouseMotion * lookSpeed * Time::frameTime);

        auto *characterComponent = m_Entity->getComponent<CharacterControllerComponent>();
        if (characterComponent == nullptr) {
            return;
        }
        characterComponent->setLookingDirection(cameraComponent->m_Camera.getViewDirection());
    }
}

std::string CharacterMouseLookBehaviour::getDescription() {
    return "Character mouse-look input";
}
