#include "JointsProcessingSystem.h"
#include "../../Common/Events/EntityLinkingEvent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem() {
    m_hingeJointComponentRegistry = useEntityUniqueComponentRegistry<PhysicsHingeJointComponent>();
    m_fixedJointComponentRegistry = useEntityUniqueComponentRegistry<PhysicsFixedJointComponent>();
    m_sliderJointComponentRegistry = useEntityUniqueComponentRegistry<PhysicsSliderJointComponent>();
}

void JointsProcessingSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    // eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleSystemEvent);
}

void JointsProcessingSystem::process(EventManager &eventManager) {
}

void JointsProcessingSystem::handleSystemEvent(const SystemEvent &event) {
    // if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
    //     recreateAll();
    // } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
    //     releaseAll();
    // }
}