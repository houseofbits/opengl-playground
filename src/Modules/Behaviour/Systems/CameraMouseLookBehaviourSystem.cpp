#include "CameraMouseLookBehaviourSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../../Core/Helper/Time.h"
#include "../Components/EntityBehaviourComponent.h"

CameraMouseLookBehaviourSystem::CameraMouseLookBehaviourSystem() : EntitySystem() {
    usesComponent<EntityBehaviourComponent>();
}

void CameraMouseLookBehaviourSystem::process() {

}

void CameraMouseLookBehaviourSystem::initialize(ResourceManager *) {

}

void CameraMouseLookBehaviourSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraMouseLookBehaviourSystem::handleInputEvent);
}

bool CameraMouseLookBehaviourSystem::handleInputEvent(InputEvent *event) {
    if (event->type != InputEvent::MOUSEMOVE || !event->mouseButtonLeft) {
        return true;
    }

    for (const auto &behaviour: getComponentContainer<EntityBehaviourComponent>()) {
        if (behaviour.second->hasActiveBehaviour(EntityBehaviourComponent::BEHAVIOUR_CAMERA_MOUSE_LOOK)) {
            auto *camera = m_EntityContext->getEntityComponent<CameraComponent>(behaviour.first);
            if (camera == nullptr) {
                Log::warn("CameraMouseLookBehaviourSystem: entity does not have CameraComponent");

                continue;
            }
            if (!camera->m_isActive) {
                continue;
            }

            float lookSpeed = 0.15;
            camera->rotateView(-event->mouseMotion * lookSpeed * Time::frameTime);
        }
    }

    return true;
}