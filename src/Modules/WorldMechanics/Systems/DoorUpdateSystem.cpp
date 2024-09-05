#include "DoorUpdateSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/DoorComponent.h"

DoorUpdateSystem::DoorUpdateSystem() : EntitySystem() {
    usesComponent<DoorComponent>();
    usesComponent<TransformComponent>();
}

void DoorUpdateSystem::process() {
    for (const auto doorItem: getComponentContainer<DoorComponent>()) {
        auto *door = doorItem.second;
        auto *transform = getComponent<TransformComponent>(doorItem.first);
        if (transform == nullptr) {
            continue;
        }

        if (door->isOpening()) {
            door->m_currentPosition += Time::frameTime * 0.5f;
            if (door->m_currentPosition >= 1.0) {
                door->setOpen();
            }
        } else if (door->isClosing()) {
            door->m_currentPosition -= Time::frameTime * 0.5f;
            if (door->m_currentPosition <= 0.0) {
                door->setClosed();
            }
        }

        if (door->m_movementType == DoorComponent::TYPE_SLIDING) {
            glm::vec3 translation = door->m_axis * (door->m_maxPosition * door->m_currentPosition);
            transform->m_transform = transform->m_initialTransform;
            transform->setTranslation(translation);
        } else {
            glm::mat4 m = transform->m_initialTransform;
            transform->m_transform = glm::rotate(m, door->m_maxPosition * door->m_currentPosition, door->m_axis);


//            glm::quat initialRotation = transform->getInitialRotation();
//            glm::quat maxRotation = glm::angleAxis(door->m_maxPosition, glm::normalize(door->m_axis)) * initialRotation;
//
//            glm::quat rotation = glm::slerp(initialRotation, maxRotation, door->m_currentPosition);
//            transform->setRotation(rotation);
        }
    }
}

void DoorUpdateSystem::initialize(ResourceManager *) {
}

void DoorUpdateSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &DoorUpdateSystem::handleCharacterPickingEvent);
}

bool DoorUpdateSystem::handleCharacterPickingEvent(CharacterPickingEvent *event) {
    auto *door = getComponent<DoorComponent>(event->m_entityId);
    if (door != nullptr && event->m_doActivate) {
        door->activate();
    }

    return true;
}
