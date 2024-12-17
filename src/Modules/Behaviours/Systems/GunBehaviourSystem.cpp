#include "GunBehaviourSystem.h"

#include "../../Common/Components/CameraComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"

GunBehaviourSystem::GunBehaviourSystem() : EntitySystem() {
    m_registry = useComponentsRegistry<TransformComponent, GunBehaviourComponent>();
}

void GunBehaviourSystem::initialize(ResourceManager &) {
}

void GunBehaviourSystem::process(EventManager &) {
    for (const auto &[id, components]: m_registry->container()) {
        const auto &[transform, behaviour] = components.get();
        if (behaviour->m_isActive && transform->isLinkedToEntityId()) {
            auto parentEntity = m_EntityContext->getEntity(transform->getLinkedEntityId());
            if (parentEntity && parentEntity->hasComponent<PhysicsCharacterComponent>()) {
                auto characterComponent = parentEntity->getComponent<PhysicsCharacterComponent>();
                auto cameraComponent = parentEntity->getComponent<CameraComponent>();
                auto transformComponent = parentEntity->getComponent<TransformComponent>();

                if (cameraComponent == nullptr || transformComponent == nullptr || !cameraComponent->m_isActive) {
                    return;
                }

                auto eyePosition = cameraComponent->getWorldPosition();

                glm::vec3 target = eyePosition + characterComponent->m_lookingDirection * 2.0f;

                auto localTarget = inverse(transformComponent->getWorldTransform()) * glm::vec4(target, 1.0);

                auto lookAtM = lookAt(
                    transform->getLocalPosition(),
                    glm::vec3(localTarget),
                    glm::vec3(0, 1, 0)
                    );

                transform->setLocalRotation(lookAtM);
            }
        }
    }
}

void GunBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    //    eventManager.registerEventReceiver(this, &GunBehaviourSystem::handleCharacterPickingEvent);
}
