#include "JointsProcessingSystem.h"
#include "../../Common//Events//EntityLinkingEvent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem() {
    m_hingeJointComponentRegistry = useComponentRegistry<PhysicsHingeJointComponent>();
    m_fixedJointComponentRegistry = useComponentRegistry<PhysicsFixedJointComponent>();
    m_sliderJointComponentRegistry = useComponentRegistry<PhysicsSliderJointComponent>();
}

void JointsProcessingSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleCharacterPickingEvent);
    eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleEntityLinkingEvent);
}

void JointsProcessingSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    for (const auto &[id, joint]: m_hingeJointComponentRegistry->container()) {
        processJoint(id, (BasePhysicsJoint*)joint);
    }
    for (const auto &[id, joint]: m_fixedJointComponentRegistry->container()) {
        processJoint(id, (BasePhysicsJoint*)joint);
    }
    for (const auto &[id, joint]: m_sliderJointComponentRegistry->container()) {
        processJoint(id, (BasePhysicsJoint*)joint);
    }
}

void JointsProcessingSystem::handleCharacterPickingEvent(const PhysicsPickingEvent &event) {
    auto *hinge = m_hingeJointComponentRegistry->get(event.m_entityId);
    if (hinge != nullptr && event.m_doActivate) {
        hinge->activate();
    }
}

void JointsProcessingSystem::processJoint(Identity::Type entityId, BasePhysicsJoint *joint) {
    if (!joint->isCreated()) {
        auto *bodyA = m_EntityContext->getEntityComponent<PhysicsBodyComponent>(entityId);
        if (bodyA == nullptr) {
            return;
        }

        auto *bodyB = m_EntityContext->findEntityComponent<PhysicsBodyComponent>(joint->getLinkedEntityName());
        if (bodyB == nullptr) {
            return;
        }

        // joint->m_targetEntityId = bodyB->m_EntityId.id();
        joint->create(*bodyA, *bodyB);
    } else {
        joint->update();
    }
}

void JointsProcessingSystem::handleEntityLinkingEvent(const EntityLinkingEvent &event) {
    for (const auto &[id, joint]: m_hingeJointComponentRegistry->container()) {
        if (joint->m_Id.id() == event.m_componentId) {
            joint->release();
        }
    }
    for (const auto &[id, joint]: m_fixedJointComponentRegistry->container()) {
        if (joint->m_Id.id() == event.m_componentId) {
            joint->release();
        }
    }
    for (const auto &[id, joint]: m_sliderJointComponentRegistry->container()) {
        if (joint->m_Id.id() == event.m_componentId) {
            joint->release();
        }
    }
}
