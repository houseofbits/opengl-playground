#include "CameraInputBehaviourSystem.h"
#include "../../../Core/Helper/Time.h"


CameraInputBehaviourSystem::CameraInputBehaviourSystem() : EntitySystem() {
    usesComponent<EntityBehaviourComponent>();
}

void CameraInputBehaviourSystem::process() {

}

void CameraInputBehaviourSystem::initialize(ResourceManager *) {

}

void CameraInputBehaviourSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraInputBehaviourSystem::handleInputEvent);
}

bool CameraInputBehaviourSystem::handleInputEvent(InputEvent *event) {
    for (const auto &behaviour: getComponentContainer<EntityBehaviourComponent>()) {
        if (!behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOVEMENT) ||
            !behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOUSE_LOOK)) {
            continue;
        }

        auto *cameraComp = m_EntityContext->getEntityComponent<CameraComponent>(behaviour.first);
        if (cameraComp == nullptr) {
            Log::warn("CameraInputBehaviourSystem: Entity does not have CameraComponent");

            continue;
        }
        if (!cameraComp->m_isActive) {
            continue;
        }

        Camera *camera = &cameraComp->m_Camera;
        glm::vec3 position = camera->getPosition();
        auto tbn = cameraComp->calculateTBN(camera->getViewDirection());
        float moveSpeed = 3;

        if (behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOVEMENT)) {
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

        if (behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOUSE_LOOK) &&
            event->type == InputEvent::MOUSEMOVE &&
            event->mouseButtonLeft) {
            float lookSpeed = 0.15;
            cameraComp->rotateView(-event->mouseMotion * lookSpeed * Time::frameTime);
        }
    }

    return true;
}
