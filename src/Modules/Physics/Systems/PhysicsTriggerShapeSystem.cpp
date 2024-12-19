#include "PhysicsTriggerShapeSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/PhysicsTriggerShapeComponent.h"

PhysicsTriggerShapeSystem::PhysicsTriggerShapeSystem() : EntitySystem() {
    m_registry = useComponentsRegistry<TransformComponent, PhysicsTriggerShapeComponent>();
}

void PhysicsTriggerShapeSystem::initialize(ResourceManager &) {
}

void PhysicsTriggerShapeSystem::process(EventManager &) {
    for (const auto &[id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        if (!body->isCreated()) {
            body->create(*transform);
        }
    }
}

void PhysicsTriggerShapeSystem::registerEventHandlers(EventManager &eventManager) {
//    eventManager.registerEventReceiver(this, &PhysicsTriggerShapeSystem::handleCharacterPickingEvent);
}
