#include "DoorBehaviourSystem.h"

DoorBehaviourSystem::DoorBehaviourSystem() : EntitySystem() {

}

void DoorBehaviourSystem::initialize(ResourceManager &) {

}

void DoorBehaviourSystem::process(EventManager &) {

}

void DoorBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &DoorBehaviourSystem::handleCharacterPickingEvent);
}

void DoorBehaviourSystem::handleCharacterPickingEvent(const PhysicsPickingEvent &) {

}
