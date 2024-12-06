#include "MainCharacterBehaviourSystem.h"
#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../../Core/Helper/Time.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"

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

    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
        auto cameraComponent = entity->getComponent<CameraComponent>();
        if (!cameraComponent || !cameraComponent->m_isActive) {
            return;
        }

        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event.mouseMotion * lookSpeed * Time::frameTime);

        auto *characterComponent = entity->getComponent<PhysicsCharacterComponent>();
        if (characterComponent == nullptr) {
            return;
        }
        characterComponent->setLookingDirection(cameraComponent->m_Camera.getViewDirection());
    }

}
