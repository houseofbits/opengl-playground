#include <unistd.h>
#include "CharacterControllerSystem.h"

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_PhysicsResource() {
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
}

void CharacterControllerSystem::initialize(ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerSystem::process(EventManager &eventManager) {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateCCTs();
}

void CharacterControllerSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CharacterControllerSystem::handleEditorUIEvent);
}

void CharacterControllerSystem::handleEditorUIEvent(const EditorUIEvent *const event) {
    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }
}

void CharacterControllerSystem::resetToInitialTransform() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        auto *transform = getComponent<TransformComponent>(component.first);
        transform->m_transform = transform->m_initialTransform;
    }
}

void CharacterControllerSystem::updateCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        auto *transform = getComponent<TransformComponent>(component.first);

        if (!component.second->isCreated()) {
            component.second->create(*transform);
        } else {
            auto *cameraComp = getComponent<CameraComponent>(component.first);
            if (cameraComp == nullptr) {
                return;
            }

            component.second->setLookingDirection(cameraComp->m_Camera.getViewDirection());
            component.second->update(*transform, !m_isSimulationDisabled);

            if (cameraComp->m_isActive) {
                cameraComp->m_Camera.setPosition(
                        transform->getTranslation() + glm::vec3(0, component.second->m_height, 0));
            }
//                PhysicsRayCastResult hit;
//                if (component.second->rayCast(cameraComp->m_Camera.position,
//                                              cameraComp->m_Camera.getViewDirection() * 10.f, hit)) {
//                    auto *e = new PhysicsPickingEvent();
//                    e->m_entityId = hit.m_entityId;
//                    e->m_distance = hit.m_distance;
//                    e->m_touchPoint = hit.m_touchPoint;
//                    e->m_doActivate = m_doInteract;
//                    m_EventManager->queueEvent(e);
//                }
//            }
        }
    }
}