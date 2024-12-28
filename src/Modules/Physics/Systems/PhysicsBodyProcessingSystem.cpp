#include "PhysicsBodyProcessingSystem.h"

PhysicsBodyProcessingSystem::PhysicsBodyProcessingSystem() : EntitySystem(),
                                                             m_PhysicsResource(),
                                                             m_isSimulationDisabled(false) {
    m_registry = useComponentsRegistry<TransformComponent, PhysicsBodyComponent>();
}

void PhysicsBodyProcessingSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsBodyProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsBodyProcessingSystem::handleEditorUIEvent);
}

void PhysicsBodyProcessingSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    for (const auto &[id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        if (!body->isCreated()) {
            body->create(*transform);
        } else {
            body->update(*transform, !m_isSimulationDisabled);
        }
    }
}

void PhysicsBodyProcessingSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
        wakeUpAll();
    } else if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event.m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }
}

void PhysicsBodyProcessingSystem::resetToInitialTransform() {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        transform->m_transform = transform->m_initialTransform;
    }
}

void PhysicsBodyProcessingSystem::wakeUpAll() {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, body] = components.get();
        body->wakeUp();
    }
}
