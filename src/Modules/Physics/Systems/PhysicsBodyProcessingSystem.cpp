#include "PhysicsBodyProcessingSystem.h"

PhysicsBodyProcessingSystem::PhysicsBodyProcessingSystem() : EntitySystem(),
                                                             m_isSimulationDisabled(false) {
    m_registry = useComponentsRegistry<TransformComponent, PhysicsBodyComponent>();
}

void PhysicsBodyProcessingSystem::initialize(ResourceManager &resourceManager, EventManager &) {
}

void PhysicsBodyProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsBodyProcessingSystem::handleSystemEvent);
}

void PhysicsBodyProcessingSystem::process(EventManager &eventManager) {
    for (const auto &[id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        body->update(*transform, !m_isSimulationDisabled);
    }
}

void PhysicsBodyProcessingSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isSimulationDisabled = false;
        recreateAll();
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isSimulationDisabled = true;
        resetToInitialTransform();
    }
}

void PhysicsBodyProcessingSystem::resetToInitialTransform() const {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        transform->m_transform = transform->m_initialTransform;
    }
}

void PhysicsBodyProcessingSystem::recreateAll() const {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        body->initialize(*m_EntityContext);
    }
}
