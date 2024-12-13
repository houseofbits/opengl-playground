#include <unistd.h>
#include "CharacterControllerSystem.h"

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_PhysicsResource() {
    m_registry = useComponentsRegistry<TransformComponent, PhysicsCharacterComponent>();
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

void CharacterControllerSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event.m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }
}

void CharacterControllerSystem::resetToInitialTransform() const {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, cct] = components.get();
        transform->m_transform = transform->m_initialTransform;
    }
}

void CharacterControllerSystem::updateCCTs() const {
    for (const auto [id, components]: m_registry->container()) {
        const auto &[transform, cct] = components.get();

        if (!cct->isCreated()) {
            cct->create(*transform);
        } else {
            cct->update(*transform, !m_isSimulationDisabled);
        }
    }
}
