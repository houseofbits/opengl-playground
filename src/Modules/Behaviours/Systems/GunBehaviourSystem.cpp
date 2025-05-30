#include "GunBehaviourSystem.h"

#include "../../Common/Components/CameraComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Behaviours/Components/MainCharacterBehaviourComponent.h"

GunBehaviourSystem::GunBehaviourSystem() : EntitySystem() {
    m_registry = useEntityUniqueComponentRegistry<GunBehaviourComponent>();
}

void GunBehaviourSystem::initialize(ResourceManager &, EventManager &) {
}

void GunBehaviourSystem::process(EventManager &) {
    const auto entity = findActiveGunEntity();
    if (entity == nullptr) {
        return;
    }
    auto joint = entity->getComponent<PhysicsSwingTwistJointComponent>();
    if (joint->isStateConnected()) {
        const auto player = m_EntityContext->findEntity(joint->m_targetEntityName);
        const auto playerBehaviour = player->getComponent<MainCharacterBehaviourComponent>();
        if (!playerBehaviour) {
            return;
        }

        joint->m_useLookAtBehaviour = true;
        joint->m_lookAtBehaviour.m_targetPosition =
                playerBehaviour->m_viewPoint + playerBehaviour->m_lookingDirection * 3.0f;
        joint->activate();
    }
}

void GunBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &GunBehaviourSystem::handlePhysicsPickingEvent);
    eventManager.registerEventReceiver(this, &GunBehaviourSystem::handlePhysicsSensorEvent);
    eventManager.registerEventReceiver(this, &GunBehaviourSystem::handleInputEvent);
}

void GunBehaviourSystem::handlePhysicsPickingEvent(const PhysicsPickingEvent &event) {
    if (m_registry->contains(event.m_entityId) && event.m_distance < 2.0) {
        const auto behaviour = m_registry->get(event.m_entityId);
        const auto entity = m_EntityContext->getEntity(behaviour->m_EntityId.id());
        auto joint = entity->getComponent<PhysicsSwingTwistJointComponent>();
        if (joint && joint->isStateDisconnected()) {
            joint->connectToEntityTarget("CharacterController", "GunAttachment");
        }
    }
}

void GunBehaviourSystem::handlePhysicsSensorEvent(const PhysicsSensorEvent &event) {
}

void GunBehaviourSystem::handleInputEvent(const InputEvent &event) {
    if (event.type == InputEvent::KEYPRESS) {
        //E - detach
        if (event.keyCode == 8) {
            const auto entity = findActiveGunEntity();
            if (entity == nullptr) {
                return;
            }

            auto joint = entity->getComponent<PhysicsSwingTwistJointComponent>();
            if (joint->isStateConnected()) {
                joint->requestDisconnectState();
            }
        }
    }
}

Entity *GunBehaviourSystem::findActiveGunEntity() const {
    for (const auto &[id, behaviour]: m_registry->container()) {
        const auto entity = m_EntityContext->getEntity(behaviour->m_EntityId.id());

        auto joint = entity->getComponent<PhysicsSwingTwistJointComponent>();
        if (joint->isStateConnected()) {
            const auto player = m_EntityContext->findEntity(joint->m_targetEntityName);
            if (!player) {
                continue;
            }

            if (!player->hasComponent<MainCharacterBehaviourComponent>()) {
                continue;
            }

            return entity;
        }
    }

    return nullptr;
}
