#include "JointsProcessingSystem.h"
#include "../../Common/Events/EntityLinkingEvent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem(), m_isSimulationDisabled(true) {
    m_physicsJoints = useSameComponentRegistry<BasePhysicsJoint>();
}

void JointsProcessingSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleSystemEvent);
}

void JointsProcessingSystem::process(EventManager &eventManager) {
    if (m_isSimulationDisabled) {
        return;
    }
    for (const auto &[id, component]: m_physicsJoints->container()) {
        if (component->isStateConnected()) {
            component->update();
        } else if (component->getState() == BasePhysicsJoint::STATE_AWAITING_CONNECTION) {
            component->createPhysics(*m_EntityContext);
        } else if (component->getState() == BasePhysicsJoint::STATE_AWAITING_DISCONNECTION) {
            component->release();
        }
    }
}

void JointsProcessingSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        for (const auto &[id, component]: m_physicsJoints->container()) {
            if (component->m_isInitiallyConnected) {
                component->requestConnectState();
            }
        }
        m_isSimulationDisabled = false;
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isSimulationDisabled = true;
        releaseAll();
    }
}

void JointsProcessingSystem::releaseAll() const {
    for (const auto &[id, component]: m_physicsJoints->container()) {
        component->release();
    }
}
