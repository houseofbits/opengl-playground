#include "CharacterInputBehaviourSystem.h"
#include "../Components/EntityBehaviourComponent.h"

CharacterInputBehaviourSystem::CharacterInputBehaviourSystem() : EntitySystem() {
    usesComponent<EntityBehaviourComponent>();
}

void CharacterInputBehaviourSystem::process() {

}

void CharacterInputBehaviourSystem::initialize(ResourceManager *) {

}

void CharacterInputBehaviourSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CharacterInputBehaviourSystem::handleInputEvent);
}

bool CharacterInputBehaviourSystem::handleInputEvent(InputEvent *event) {
    for (const auto &behaviour: getComponentContainer<EntityBehaviourComponent>()) {



    }
    return true;
}
