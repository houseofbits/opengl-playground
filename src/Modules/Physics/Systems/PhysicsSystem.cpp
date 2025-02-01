#include "PhysicsSystem.h"
#include "../Events//PhysicsTriggerShapeEvent.h"

PhysicsSystem::PhysicsSystem() : EntitySystem(),
                                 m_isSimulationDisabled(false),
                                 m_PhysicsResource() {
}

void PhysicsSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    m_PhysicsResource().m_sensorContacts.clear();
    m_PhysicsResource().clearEntityContacts();
    if (!m_isSimulationDisabled) {
        m_PhysicsResource().simulate();
    }

    for (const auto &[sensorEntityId, colliderEntityId]: m_PhysicsResource().m_sensorContacts) {
        eventManager.queueEvent<PhysicsTriggerShapeEvent>(sensorEntityId, colliderEntityId);
    }
}

void PhysicsSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsSystem::handleSystemEvent);
}

void PhysicsSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isSimulationDisabled = false;
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isSimulationDisabled = true;
    }
}
