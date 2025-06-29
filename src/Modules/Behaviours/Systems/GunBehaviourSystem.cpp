#include "GunBehaviourSystem.h"

#include "../../Common/Components/CameraComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Behaviours/Components/MainCharacterBehaviourComponent.h"
#include "../../Physics/Components/PhysicsDistanceJointComponent.h"
#include "../../Physics/Components/PhysicsFixedJointComponent.h"

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
    if (m_registry->contains(event.m_entityId) && event.m_distance < 5.0) {
        const auto behaviour = m_registry->get(event.m_entityId);
        const auto entity = m_EntityContext->getEntity(behaviour->m_EntityId.id());
        auto joint = entity->getComponent<PhysicsSwingTwistJointComponent>();

        const auto activeGun = findActiveGunEntity();
        if (activeGun == nullptr && joint) {
            if (joint->isStateDisconnected()) {
                joint->connectToEntityTarget("CharacterController", "GunAttachment");
            }
        }
    }

    if (auto target = m_EntityContext->getEntity(event.m_entityId)) {
        const auto gun = findActiveGunEntity();
        if (gun) {
            if (auto magnetGun = gun->getComponent<PhysicsDistanceJointComponent>("magnetGun")) {
                if (magnetGun->isStateDisconnected()) {
                    if (const auto transform = target->getComponent<TransformComponent>()) {
                        const auto localPoint = transform->getInverseModelMatrix() * glm::vec4(event.m_touchPoint, 1.0);
                        magnetGun->m_localAttachmentMatrixB = glm::translate(glm::mat4(1.0), glm::vec3(localPoint));
                    }
                    magnetGun->connectToEntity(target->m_Name);
                } else {
                    magnetGun->requestDisconnectState();
                }
            }
            if (auto chainLinkGun = gun->getComponent<PhysicsDistanceJointComponent>("chainLinkGun")) {
                if (auto chainLinkEnd = m_EntityContext->findEntityComponent<PhysicsDistanceJointComponent>("ChainLink4")) {
                    if (chainLinkEnd->isStateDisconnected()) {
                        if (const auto transform = target->getComponent<TransformComponent>()) {
                            const auto localPoint = transform->getInverseModelMatrix() * glm::vec4(event.m_touchPoint, 1.0);
                            chainLinkEnd->m_localAttachmentMatrixB = glm::translate(glm::mat4(1.0), glm::vec3(localPoint));
                        }
                        chainLinkEnd->connectToEntity(target->m_Name);
                    } else {
                        chainLinkEnd->requestDisconnectState();
                    }
                }
            }
            if (auto fixedGun = gun->getComponent<PhysicsFixedJointComponent>("fixedGun")) {
                if (fixedGun->isStateDisconnected()) {
                    // if (const auto transform = target->getComponent<TransformComponent>()) {
                    //     const auto localPoint = transform->getInverseModelMatrix() * glm::vec4(event.m_touchPoint, 1.0);
                    //     fixedtGun->m_localAttachmentMatrixB = glm::translate(glm::mat4(1.0), glm::vec3(localPoint));
                    // }
                    fixedGun->connectToEntity(target->m_Name);
                } else {
                    fixedGun->requestDisconnectState();
                }
            }
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
