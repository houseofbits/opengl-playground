#include "PhysicsBodyProcessingSystem.h"
#include "../Components/PhysicsBodyComponent.h"
#include "../Helpers/PhysicsTypeCast.h"

PhysicsBodyProcessingSystem::PhysicsBodyProcessingSystem() : EntitySystem(),
                                                             m_PhysicsResource(),
                                                             m_isSimulationDisabled(false) {
    usesComponent<PhysicsBodyComponent>();
    usesComponent<TransformComponent>();
}

void PhysicsBodyProcessingSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void PhysicsBodyProcessingSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &PhysicsBodyProcessingSystem::handleEditorUIEvent);
}

void PhysicsBodyProcessingSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    for (const auto body: getComponentContainer<PhysicsBodyComponent>()) {
        auto *transform = getComponent<TransformComponent>(body.first);
        if (body.second->m_rigidBody == nullptr) {
            body.second->create(*transform);
        } else {
            body.second->update(*transform, !m_isSimulationDisabled);
        }
    }
}

bool PhysicsBodyProcessingSystem::handleEditorUIEvent(EditorUIEvent *event) {
    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }

    return true;
}

void PhysicsBodyProcessingSystem::resetToInitialTransform() {
    for (const auto body: getComponentContainer<PhysicsBodyComponent>()) {
        auto *transform = getComponent<TransformComponent>(body.first);
        transform->m_transform = transform->m_initialTransform;
    }
}
