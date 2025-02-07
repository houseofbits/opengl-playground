#include <unistd.h>
#include "CharacterControllerSystem.h"

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_PhysicsResource() {
    m_registry = useEntityRelatedComponentsRegistry<TransformComponent, PhysicsCharacterComponent>();
}

void CharacterControllerSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateCCTs();
}

void CharacterControllerSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterControllerSystem::handleSystemEvent);
}

void CharacterControllerSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isSimulationDisabled = false;
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
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
