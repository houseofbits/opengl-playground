#include "GunBehaviourSystem.h"

#include "../../Common/Components/CameraComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Behaviours/Components/MainCharacterBehaviourComponent.h"

GunBehaviourSystem::GunBehaviourSystem() : EntitySystem() {
    m_registry = useEntityRelatedComponentsRegistry<PhysicsSwingTwistJointComponent, GunBehaviourComponent>();
}

void GunBehaviourSystem::initialize(ResourceManager &, EventManager &) {
}

void GunBehaviourSystem::process(EventManager &) {
    for (const auto &[id, components]: m_registry->container()) {
        const auto &[lookAtJoint, behaviour] = components.get();
        if (lookAtJoint->isStateConnected()) {
            // auto entity = m_EntityContext->findEntity(lookAtJoint->m_targetEntityBName);
            // if (!entity) {
            //     continue;
            // }

            // if (!entity->hasComponent<MainCharacterBehaviourComponent>()) {
            //     continue;
            // }

            auto player = m_EntityContext->findEntity("CharacterController");
            if (!player) {
                continue;
            }

            // auto playerBehaviour = player->getComponent<MainCharacterBehaviourComponent>();
            // if (!playerBehaviour) {
            //     continue;
            // }

            if (auto testTargetEntity = m_EntityContext->findEntity("TestTarget1")) {
                const auto t = testTargetEntity->getComponent<TransformComponent>();

                lookAtJoint->m_useLookAtBehaviour = true;
                lookAtJoint->m_lookAtBehaviour.m_targetPosition = t->getWorldPosition();
                // lookAtJoint->m_lookAtBehaviour.m_targetPosition  = playerBehaviour->m_viewPoint + playerBehaviour->m_lookingDirection * 3.0f;
                lookAtJoint->activate();
            }
        }

        // if (behaviour->m_isActive && transform->isLinkedToEntityId()) {
        //     auto parentEntity = m_EntityContext->getEntity(transform->getLinkedEntityId());
        //     if (parentEntity && parentEntity->hasComponent<PhysicsCharacterComponent>()) {
        //         auto characterComponent = parentEntity->getComponent<PhysicsCharacterComponent>();
        //         auto cameraComponent = parentEntity->getComponent<CameraComponent>();
        //         auto transformComponent = parentEntity->getComponent<TransformComponent>();
        //         auto characterBehaviour = parentEntity->getComponent<MainCharacterBehaviourComponent>();
        //
        //         if (characterBehaviour == nullptr || transformComponent == nullptr || !characterBehaviour->m_isActive) {
        //             return;
        //         }
        //
        //         // auto eyePosition = characterBehaviour->m_cameraAttachmentPosition; //Get world position
        //         //
        //         // glm::vec3 target = eyePosition + characterBehaviour->m_lookingDirection * 2.0f;
        //         //
        //         // auto localTarget = inverse(transformComponent->getWorldTransform()) * glm::vec4(target, 1.0);
        //         //
        //         // auto lookAtM = lookAt(
        //         //     transform->getLocalPosition(),
        //         //     glm::vec3(localTarget),
        //         //     glm::vec3(0, 1, 0)
        //         //     );
        //         //
        //         // transform->setLocalRotation(lookAtM);
        //     }
        // }
    }
}

void GunBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    //    eventManager.registerEventReceiver(this, &GunBehaviourSystem::handleCharacterPickingEvent);
}
