#include <unistd.h>
#include "CharacterControllerSystem.h"
#include "../../../Core/Helper/StringUtils.h"
#include "../../../Core/Helper/Time.h"
#include "../../../Core/Helper/Types.h"
#include "../Events/PhysicsPickingEvent.h"
#include "../Helpers/PhysicsTypeCast.h"

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_movementDirection(),
                                                         m_isOnGround(false),
                                                         m_doMove(false),
                                                         m_doJump(false),
                                                         m_doInteract(false),
                                                         m_jumpPower(0),
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
    eventManager.registerEventReceiver(this, &CharacterControllerSystem::handleInputEvent);
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

void CharacterControllerSystem::handleInputEvent(const InputEvent *const event) {
    for (const auto comp: getComponentContainer<CharacterControllerComponent>()) {
        auto *cameraComp = getComponent<CameraComponent>(comp.first);
//        if (cameraComp && cameraComp->m_isActive) {
        processCCTInput(cameraComp, comp.second, event);
//        }
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
        glm::vec3 position = transform->getTranslation();

        if (!component.second->isCreated()) {
            component.second->create(*transform);
        } else {
            auto *cameraComp = getComponent<CameraComponent>(component.first);
            if (cameraComp == nullptr) {
                return;
            }

            component.second->setLookingDirection(cameraComp->m_Camera.getViewDirection());
            component.second->update(*transform, !m_isSimulationDisabled);

            if (!m_isSimulationDisabled) {
                if (m_doMove) {
                    component.second->move(m_movementDirection);
                    m_movementDirection = glm::vec3(0);
                    m_doMove = false;
                }
//
////                float dst = glm::length(transform->getTranslation() - prevPos);
////                Log::write("Speed: ", dst / (Time::timestamp - prevDt));
////                prevPos = transform->getTranslation();
////                prevDt = Time::timestamp;

//                Log::write(component.second->getVelocity());
            }

            if (cameraComp->m_isActive) {
                cameraComp->m_Camera.setPosition(
                        transform->getTranslation() + glm::vec3(0, component.second->m_height, 0));

                PhysicsRayCastResult hit;
                if (component.second->rayCast(cameraComp->m_Camera.position,
                                              cameraComp->m_Camera.getViewDirection() * 10.f, hit)) {
                    auto *e = new PhysicsPickingEvent();
                    e->m_entityId = hit.m_entityId;
                    e->m_distance = hit.m_distance;
                    e->m_touchPoint = hit.m_touchPoint;
                    e->m_doActivate = m_doInteract;
                    m_EventManager->queueEvent(e);
                }
            }
        }
    }

    m_doInteract = false;
}

void CharacterControllerSystem::processCCTInput(CameraComponent *camera, CharacterControllerComponent *cct,
                                                const InputEvent *const event) {
    glm::vec3 viewDirection = camera->m_Camera.getViewDirection();
    viewDirection.y = 0;
    m_direction = glm::normalize(viewDirection);

    glm::vec3 rightDirection = glm::normalize(glm::cross(m_direction, glm::vec3(0, 1, 0)));

    //W
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
        m_doMove = true;
        m_movementDirection = m_movementDirection + m_direction;
    }
    //S
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
        m_doMove = true;
        m_movementDirection = m_movementDirection - m_direction;
    }
    //A
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
        m_doMove = true;
        m_movementDirection = m_movementDirection - rightDirection;
    }
    //D
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
        m_doMove = true;
        m_movementDirection = m_movementDirection + rightDirection;
    }
    //Space-bar
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 44) {
        m_doJump = true;
        m_jumpPower = 30;
    }

    if (event->type == InputEvent::MOUSEDOWN && event->mouseButtonRight) {
        m_doInteract = true;
    }

    if (m_doMove) {
        m_movementDirection = glm::normalize(m_movementDirection);
    }
}