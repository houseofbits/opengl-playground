#include "MainCharacterBehaviourSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../../Application/Events/InputEvent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"

MainCharacterBehaviourSystem::MainCharacterBehaviourSystem() : EntitySystem(), m_isEditorModeEnabled(false) {
    m_registry = useEntityRelatedComponentsRegistry<TransformComponent, PhysicsCharacterComponent,
        MainCharacterBehaviourComponent>();
}

void MainCharacterBehaviourSystem::initialize(ResourceManager &, EventManager &) {
}

void MainCharacterBehaviourSystem::process(EventManager &) {
}

void MainCharacterBehaviourSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MainCharacterBehaviourSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &MainCharacterBehaviourSystem::handleSystemEvent);
}

void MainCharacterBehaviourSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isEditorModeEnabled = true;
    } else if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isEditorModeEnabled = false;
    }
}

void MainCharacterBehaviourSystem::handleInputEvent(const InputEvent &event) {
    for (const auto &[id, components]: m_registry->container()) {
        const auto &[transform, character, behaviour] = components.get();

        if (behaviour->m_isActive && !m_isEditorModeEnabled) {
            handleMouseLook(event, behaviour, character);
            handleMovement(event, behaviour, character);

            const glm::vec3 worldPosition = transform->getWorldTransform() * glm::vec4(
                                                behaviour->m_cameraAttachmentPosition, 1.0);

            // Log::write(character->m_);

            handleAction(event, character, worldPosition, behaviour->m_lookingDirection);

            updateCamera(behaviour, worldPosition);
        }
    }
}

void MainCharacterBehaviourSystem::handleMouseLook(const InputEvent &event,
                                                   MainCharacterBehaviourComponent *behaviour,
                                                   PhysicsCharacterComponent *physics) {
    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
        behaviour->adjustLookingDirection(-event.mouseMotion * behaviour->m_mouseLookSpeed * Time::frameTime);
        physics->setMoveDirection(behaviour->m_lookingDirection);
    }
}

void MainCharacterBehaviourSystem::handleMovement(const InputEvent &event,
                                                  const MainCharacterBehaviourComponent *behaviour,
                                                  PhysicsCharacterComponent *characterComponent) {
    if (event.type == InputEvent::KEYPRESS) {
        glm::vec3 forwardDirection = behaviour->m_lookingDirection;
        forwardDirection.y = 0;
        forwardDirection = glm::normalize(forwardDirection);
        glm::vec3 rightDirection = glm::normalize(glm::cross(forwardDirection, glm::vec3(0, 1, 0)));
        bool doMove = false;
        glm::vec3 movementDirection(0);

        //W
        if (event.keyCode == 26) {
            doMove = true;
            movementDirection = movementDirection + forwardDirection;
        }
        //S
        if (event.keyCode == 22) {
            doMove = true;
            movementDirection = movementDirection - forwardDirection;
        }
        //A
        if (event.keyCode == 4) {
            doMove = true;
            movementDirection = movementDirection - rightDirection;
        }
        //D
        if (event.keyCode == 7) {
            doMove = true;
            movementDirection = movementDirection + rightDirection;
        }
        //Space-bar
        //    if (event->type == InputEvent::KEYPRESS && event->keyCode == 44) {
        //        doMove = true;
        //    }

        if (doMove) {
            characterComponent->move(glm::normalize(movementDirection));
        }
    }
}

void MainCharacterBehaviourSystem::handleAction(const InputEvent &event,
                                                PhysicsCharacterComponent *characterComponent,
                                                const glm::vec3 viewPosition,
                                                const glm::vec3 viewDirection) const {
    if ((event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) ||
        (event.type == InputEvent::MOUSEUP && event.mouseButtonRight)) {
        PhysicsRayCastResult hit;
        if (characterComponent->rayCast(viewPosition,
                                        viewDirection * 10.f, hit)) {
            if (const auto entity = m_EntityContext->getEntity(hit.m_entityId)) {
                event.m_EventManager->queueEvent<PhysicsPickingEvent>(
                    hit.m_entityId,
                    entity->m_Name,
                    hit.m_distance,
                    hit.m_touchPoint,
                    event.mouseButtonRight,
                    hit.m_shapeComponentId,
                    hit.m_shapeComponentName
                );
            }
        }
    }
}

void MainCharacterBehaviourSystem::updateCamera(const MainCharacterBehaviourComponent *behaviour,
                                                const glm::vec3 viewPosition) const {
    CameraComponent *camera = nullptr;
    if (behaviour->m_cameraEntityId == 0) {
        //Try to get camera component from Self
        camera = m_EntityContext->getEntityComponent<CameraComponent>(behaviour->m_EntityId.id());
    } else {
        //Detached camera component
        camera = m_EntityContext->getEntityComponent<CameraComponent>(behaviour->m_cameraEntityId);
    }

    if (camera && camera->m_isActive) {
        camera->setPositionAndDirection(viewPosition, behaviour->m_lookingDirection, glm::vec3(0, 1, 0));
    }
}
