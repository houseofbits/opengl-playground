#include "MotorBehaviourSystem.h"

MotorBehaviourSystem::MotorBehaviourSystem() : EntitySystem() {
    m_hingeJointComponentRegistry = useEntityUniqueComponentRegistry<PhysicsHingeJointComponent>();
    m_motorBehaviourComponentRegistry = useEntityUniqueComponentRegistry<MotorBehaviourComponent>();
}

void MotorBehaviourSystem::initialize(ResourceManager &, EventManager &) {
}

void MotorBehaviourSystem::process(EventManager &) {
}

void MotorBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MotorBehaviourSystem::handleInputEvent);
}

void MotorBehaviourSystem::handleInputEvent(const InputEvent &event) {
    if (event.type == InputEvent::KEYUP) {
        //Q
        if (event.keyCode == 20) {
            for (const auto [entityId, behaviour]: m_motorBehaviourComponentRegistry->container()) {
                if (const auto joint = m_hingeJointComponentRegistry->get(entityId)) {
                    if (behaviour->m_motorOn) {
                        joint->setMotorOff();
                        behaviour->m_motorOn = false;
                        Log::write("motor off");
                    } else {
                        joint->setMotorVelocity(2.5);
                        behaviour->m_motorOn = true;

                        Log::write("motor on");
                    }
                }
            }
        }
    }
}
