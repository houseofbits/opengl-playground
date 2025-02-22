#include "PhysicsSystem.h"
#include "../Events//PhysicsTriggerShapeEvent.h"

PhysicsSystem::PhysicsSystem() : EntitySystem(),
                                 m_isSimulationDisabled(false),
                                 m_physicsComponentRegistry(nullptr),
                                 m_PhysicsResource() {
    m_physicsComponentRegistry = useSameComponentRegistry<PhysicsComponent>();
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
        processCreation();
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
        recreateAll();
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isSimulationDisabled = true;
        m_PhysicsResource().destroyAllBodies();
    }
}

void PhysicsSystem::processCreation() const {
    for (const auto [id, component]: m_physicsComponentRegistry->container()) {
        if (component->shouldCreatePhysics() && component->isReadyToCreate(*m_EntityContext)) {
            component->createPhysics(*m_EntityContext);
            component->setPhysicsCreated(true);
        }
    }
}

void PhysicsSystem::recreateAll() const {
    for (const auto [id, component]: m_physicsComponentRegistry->container()) {
        component->setPhysicsCreated(false);
    }
}

