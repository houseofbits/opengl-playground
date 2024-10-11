#include <unistd.h>
#include "CharacterControllerSystem.h"

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_PhysicsResource() {
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
}

void CharacterControllerSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateCCTs();
}

void CharacterControllerSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterControllerSystem::handleEditorUIEvent);
}

void CharacterControllerSystem::handleEditorUIEvent(const EditorUIEvent *const event) {
    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }
}

void CharacterControllerSystem::resetToInitialTransform() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        auto *transform = getComponent<TransformComponent>(component.first);
        transform->m_transform = transform->m_initialTransform;
    }
}

void CharacterControllerSystem::updateCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        auto *transform = getComponent<TransformComponent>(component.first);

        if (!component.second->isCreated()) {
            component.second->create(*transform);
        } else {
            component.second->update(*transform, !m_isSimulationDisabled);
        }
    }
}