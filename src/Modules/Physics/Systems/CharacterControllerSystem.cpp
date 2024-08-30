#include "CharacterControllerSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../../Core/Helper/Types.h"

static physx::PxControllerFilters filters;

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_PhysicsResource() {
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
}

void CharacterControllerSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void CharacterControllerSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateCCTs();
    createCCTs();
}

void CharacterControllerSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CharacterControllerSystem::handleEditorUIEvent);
    eventManager->registerEventReceiver(this, &CharacterControllerSystem::handleInputEvent);
}

bool CharacterControllerSystem::handleEditorUIEvent(EditorUIEvent *event) {
    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }

    return true;
}

bool CharacterControllerSystem::handleInputEvent(InputEvent *event) {
    for (const auto comp: getComponentContainer<CharacterControllerComponent>()) {
        auto *cameraComp = getComponent<CameraComponent>(comp.first);
        if (cameraComp && cameraComp->m_isActive) {
            processCCTInput(cameraComp, comp.second, event);
        }
    }

    return true;
}

void CharacterControllerSystem::resetToInitialTransform() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController != nullptr) {
            auto *transform = getComponent<TransformComponent>(component.first);

            transform->resetTransform();
            transform->setTranslation(component.second->m_initialPosition);
            component.second->setPhysicsPosition(component.second->m_initialPosition);
        }
    }
}

void CharacterControllerSystem::createCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController == nullptr) {
            auto *transform = getComponent<TransformComponent>(component.first);
            component.second->create(*transform);
        }
    }
}

void CharacterControllerSystem::updateCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController != nullptr) {
            component.second->m_CCTController->move(physx::PxVec3(0, -0.1, 0), 0, Time::frameTime, filters);

            auto *transform = getComponent<TransformComponent>(component.first);
            if (m_isSimulationDisabled) {
                component.second->setPhysicsPosition(transform->getTranslation());
            } else {
                transform->resetTransform();
                transform->setTranslation(component.second->getPhysicsPosition());

                auto *cameraComp = getComponent<CameraComponent>(component.first);
                if (cameraComp && cameraComp->m_isActive) {
                    cameraComp->m_Camera.setPosition(transform->getTranslation() + glm::vec3(0, component.second->m_height, 0));
                }
            }
        }
    }
}

void CharacterControllerSystem::processCCTInput(CameraComponent *camera, CharacterControllerComponent *cct, InputEvent *event) {




    glm::vec3 dir = camera->m_Camera.getViewDirection();
    dir.y = 0;
    dir = glm::normalize(dir);

    glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));

    bool doMove = false;
    physx::PxVec3 direction(0);

    //Up
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 82) {
        direction = direction + Types::GLMtoPxVec3(dir);
        doMove = true;
    }
    //Down
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 81) {
        direction = direction - Types::GLMtoPxVec3(dir);
        doMove = true;
    }
    //Left
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 80) {
        direction = direction - Types::GLMtoPxVec3(right);
        doMove = true;
    }
    //Right
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 79) {
        direction = direction + Types::GLMtoPxVec3(right);
        doMove = true;
    }

    if (doMove) {
        direction.normalize();
        direction = direction * 0.1;

        cct->m_CCTController->move(direction, 0, Time::frameTime, filters);
    }
}