#include "DoorBehaviourSystem.h"

DoorBehaviourSystem::DoorBehaviourSystem() : EntitySystem() {
    m_sliderJointComponentRegistry = useEntityRelatedComponentsRegistry<DoorBehaviourComponent, PhysicsSliderJointComponent>();
    m_hingeJointComponentRegistry = useEntityRelatedComponentsRegistry<DoorBehaviourComponent, PhysicsHingeJointComponent>();
}

void DoorBehaviourSystem::initialize(ResourceManager &, EventManager&) {
}

void DoorBehaviourSystem::process(EventManager &) {
    for (const auto &[id, components]: m_sliderJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();

        if (!joint->isInitialized()) {
            continue;
        }

        processJointBehaviour(joint, behaviour);
    }

    for (const auto &[id, components]: m_hingeJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();

        if (!joint->isInitialized()) {
            continue;
        }

        processJointBehaviour(joint, behaviour);
    }
}

void DoorBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &DoorBehaviourSystem::handleCharacterPickingEvent);
    eventManager.registerEventReceiver(this, &DoorBehaviourSystem::handlePhysicsTriggerShapeEvent);
}

void DoorBehaviourSystem::handleCharacterPickingEvent(const PhysicsPickingEvent &event) {
    for (const auto &[id, components]: m_sliderJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();
        if (behaviour->getLinkedEntityId() == event.m_entityId && event.m_doActivate) {
            handleActivation(joint, behaviour);
        }
    }

    for (const auto &[id, components]: m_hingeJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();
        if (behaviour->getLinkedEntityId() == event.m_entityId && event.m_doActivate) {
            handleActivation(joint, behaviour);
        }
    }
}

void DoorBehaviourSystem::handlePhysicsTriggerShapeEvent(const PhysicsTriggerShapeEvent &event) {
    for (const auto &[id, components]: m_sliderJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();
        if (behaviour->getLinkedEntityId() == event.m_sensorEntityId) {
            handleActivation(joint, behaviour);
        }
    }

    for (const auto &[id, components]: m_hingeJointComponentRegistry->container()) {
        const auto &[behaviour, joint] = components.get();
        if (behaviour->getLinkedEntityId() == event.m_sensorEntityId) {
            handleActivation(joint, behaviour);
        }
    }
}

void DoorBehaviourSystem::handleActivation(BasePhysicsJoint *joint, DoorBehaviourComponent *behaviour) {
    if (behaviour->m_state == DoorBehaviourComponent::STATE_CLOSED) {
        behaviour->m_state = DoorBehaviourComponent::STATE_OPENING;
        if (behaviour->m_isFixedOnFinalState) {
            joint->unLock();
        }
    } else if (behaviour->m_state == DoorBehaviourComponent::STATE_OPEN) {
        behaviour->m_state = DoorBehaviourComponent::STATE_CLOSING;
        if (behaviour->m_isFixedOnFinalState) {
            joint->unLock();
        }
    }
}

void DoorBehaviourSystem::processJointBehaviour(BasePhysicsJoint *joint, DoorBehaviourComponent *behaviour) {
    float threshold = 0.01;
    float position = joint->getUnitPosition();
    if (behaviour->m_state == DoorBehaviourComponent::STATE_OPENING) {
        if (position > 1.0 - threshold) {
            joint->setMotorOff();
            behaviour->m_state = DoorBehaviourComponent::STATE_OPEN;
            if (behaviour->m_isFixedOnFinalState) {
                joint->lockInPlace();
            }
        } else {
            joint->setMotorVelocity(behaviour->m_velocity);
            joint->activate();
        }
    }
    if (behaviour->m_state == DoorBehaviourComponent::STATE_CLOSING) {
        if (position < threshold) {
            joint->setMotorOff();
            behaviour->m_state = DoorBehaviourComponent::STATE_CLOSED;
            if (behaviour->m_isFixedOnFinalState) {
                joint->lockInPlace();
            }
        } else {
            joint->setMotorVelocity(-behaviour->m_velocity);
            joint->activate();
        }
    }
}
