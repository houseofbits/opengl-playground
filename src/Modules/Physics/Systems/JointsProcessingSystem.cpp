#include "JointsProcessingSystem.h"
#include "../Components/PhysicsJointComponent.h"

JointsProcessingSystem::JointsProcessingSystem() : EntitySystem() {
    usesComponent<PhysicsJointComponent>();
}

void JointsProcessingSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void JointsProcessingSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &JointsProcessingSystem::handleCharacterPickingEvent);
}

void JointsProcessingSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    for (const auto& body: getComponentContainer<PhysicsJointComponent>()) {
        if (!body.second->isCreated()) {
            auto *bodyA = m_EntityContext->getEntityComponent<PhysicsBodyComponent>(body.first);
            if (bodyA == nullptr) {
                continue;
            }

            auto *bodyB = m_EntityContext->findEntityComponent<PhysicsBodyComponent>(body.second->m_targetEntityName);
            if (bodyB == nullptr) {
                continue;
            }

            body.second->create(*bodyA, *bodyB);
        } else {
            body.second->update();
        }
    }
}

bool JointsProcessingSystem::handleCharacterPickingEvent(CharacterPickingEvent *event) {
    auto *hinge = getComponent<PhysicsJointComponent>(event->m_entityId);
    if (hinge != nullptr && event->m_doActivate) {
        hinge->activate();
    }

    return true;
}
