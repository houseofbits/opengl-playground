#include "JointsProcessingSystem.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../Components/PhysicsFixedJointComponent.h"
#include "../Components/PhysicsSliderJointComponent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem() {
    usesComponent<PhysicsHingeJointComponent>();
    usesComponent<PhysicsFixedJointComponent>();
    usesComponent<PhysicsSliderJointComponent>();
}

void JointsProcessingSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &JointsProcessingSystem::handleCharacterPickingEvent);
}

void JointsProcessingSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    for (const auto &joint: getComponentContainer<PhysicsHingeJointComponent>()) {
        processJoint(joint.first, (BasePhysicsJoint*)joint.second);
    }
    for (const auto &joint: getComponentContainer<PhysicsFixedJointComponent>()) {
        processJoint(joint.first, (BasePhysicsJoint*)joint.second);
    }
    for (const auto &joint: getComponentContainer<PhysicsSliderJointComponent>()) {
        processJoint(joint.first, (BasePhysicsJoint*)joint.second);
    }
}

void JointsProcessingSystem::handleCharacterPickingEvent(const PhysicsPickingEvent &event) {
    auto *hinge = getComponent<PhysicsHingeJointComponent>(event.m_entityId);
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

        auto *bodyB = m_EntityContext->findEntityComponent<PhysicsBodyComponent>(joint->m_targetEntityName);
        if (bodyB == nullptr) {
            return;
        }

        joint->m_targetEntityId = bodyB->m_EntityId.id();
        joint->create(*bodyA, *bodyB);
    } else {
        joint->update();
    }
}
