#include "CharacterMouseLookBehaviour.h"
#include "../../../Core/Helper/Time.h"
#include "../../Common/Components/CameraComponent.h"

CharacterMouseLookBehaviour::CharacterMouseLookBehaviour() : EntityBehaviour() {

}

void CharacterMouseLookBehaviour::initialize(ResourceManager &) {

}

void CharacterMouseLookBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterMouseLookBehaviour::handleInputEvent);
}

void CharacterMouseLookBehaviour::handleInputEvent(const InputEvent *const event) {
    auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
    if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
        return;
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event->mouseMotion * lookSpeed * Time::frameTime);
    }
}

std::string CharacterMouseLookBehaviour::getDescription() {
    return "";
}
