#include "JointsProcessingSystem.h"
#include "../../Common/Events/EntityLinkingEvent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem() {
    m_hingeJointComponentRegistry = useComponentRegistry<PhysicsHingeJointComponent>();
    m_fixedJointComponentRegistry = useComponentRegistry<PhysicsFixedJointComponent>();
    m_sliderJointComponentRegistry = useComponentRegistry<PhysicsSliderJointComponent>();
}

void JointsProcessingSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleSystemEvent);
}

void JointsProcessingSystem::process(EventManager &eventManager) {

}

void JointsProcessingSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        recreateAll();
    }
}

void JointsProcessingSystem::recreateAll() const {
    for (const auto &[id, joint]: m_fixedJointComponentRegistry->container()) {
        joint->initialize(*m_EntityContext);
    }
    for (const auto &[id, joint]: m_hingeJointComponentRegistry->container()) {
        joint->initialize(*m_EntityContext);
    }
    for (const auto &[id, joint]: m_sliderJointComponentRegistry->container()) {
        joint->initialize(*m_EntityContext);
    }
}