#include "CameraMovementBehaviourSystem.h"
#include "../Components/EntityBehaviourComponent.h"
#include "../../../Core/Helper/Time.h"


CameraMovementBehaviourSystem::CameraMovementBehaviourSystem() : EntitySystem() {
    usesComponent<EntityBehaviourComponent>();
}

void CameraMovementBehaviourSystem::process() {

}

void CameraMovementBehaviourSystem::initialize(ResourceManager *) {

}

void CameraMovementBehaviourSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraMovementBehaviourSystem::handleInputEvent);
}

bool CameraMovementBehaviourSystem::handleInputEvent(InputEvent *event) {
    for (const auto &behaviour: getComponentContainer<EntityBehaviourComponent>()) {
        if (behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOVEMENT)) {
            auto *cameraComp = m_EntityContext->getEntityComponent<CameraComponent>(behaviour.first);
            if (cameraComp == nullptr) {
                Log::warn("CameraMovementBehaviourSystem: entity does not have CameraComponent");

                continue;
            }
            if (!cameraComp->m_isActive) {
                continue;
            }

            Camera *camera = &cameraComp->m_Camera;
            glm::vec3 position = camera->getPosition();
            auto tbn = cameraComp->calculateTBN(camera->getViewDirection());
            float moveSpeed = 3;

            if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonRight) {
                position += tbn.up * (moveSpeed * event->mouseMotion.y * Time::frameTime);
                position -= tbn.right * (moveSpeed * event->mouseMotion.x * Time::frameTime);
                camera->setPosition(position);
                camera->setView(tbn.view, tbn.up);
            }

            if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
                position += tbn.view * (moveSpeed * Time::frameTime);
                camera->setPosition(position);
                camera->setView(tbn.view, tbn.up);
            }

            if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
                position -= tbn.view * (moveSpeed * Time::frameTime);
                camera->setPosition(position);
                camera->setView(tbn.view, tbn.up);
            }

            if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
                position -= tbn.right * (moveSpeed * Time::frameTime);
                camera->setPosition(position);
                camera->setView(tbn.view, tbn.up);
            }

            if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
                position += tbn.right * (moveSpeed * Time::frameTime);
                camera->setPosition(position);
                camera->setView(tbn.view, tbn.up);
            }

        }
    }

    return true;
}
