#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() : EntitySystem(),
                                 m_isSimulationDisabled(false),
                                 m_PhysicsResource() {
}

void PhysicsSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    m_PhysicsResource().clearEntityContacts();
    if (!m_isSimulationDisabled) {
        m_PhysicsResource().simulate();
    }
}

void PhysicsSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &PhysicsSystem::handleEditorUIEvent);
}

void PhysicsSystem::handleEditorUIEvent(const EditorUIEvent & event) {
    if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event.m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event.m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        m_isSimulationDisabled = true;
    }
}
