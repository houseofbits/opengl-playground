#include "MainCharacterBehaviourComponent.h"
#include "../Systems/MainCharacterBehaviourSystem.h"

MainCharacterBehaviourComponent::MainCharacterBehaviourComponent() : Component(),
                                                                     m_isActive(true),
                                                                     m_mouseLookSpeed(0.15),
                                                                     m_movementSpeed(10.0),
                                                                     m_cameraAttachmentPosition(0.0),
                                                                     m_cameraEntityId(0),
                                                                     m_lookingDirection(1, 0, 0) {
}

void MainCharacterBehaviourComponent::serialize(nlohmann::json &j) {
    j[ACTIVE_KEY] = m_isActive;
    j[MLOOK_SPEED_KEY] = m_mouseLookSpeed;
    j[MOVEMENT_SPEED_KEY] = m_movementSpeed;
    j[CAMERA_NAME_KEY] = m_cameraName;
    j[CAMERA_ATTACHMENT_KEY] = m_cameraAttachmentPosition;
    j[LOOK_DIRECTION_KEY] = m_lookingDirection;
}

void MainCharacterBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_isActive = j.value(ACTIVE_KEY, m_isActive);
    m_mouseLookSpeed = j.value(MLOOK_SPEED_KEY, m_mouseLookSpeed);
    m_movementSpeed = j.value(MOVEMENT_SPEED_KEY, m_movementSpeed);
    m_cameraName = j.value(CAMERA_NAME_KEY, "");
    m_cameraAttachmentPosition = j.value(CAMERA_ATTACHMENT_KEY, m_cameraAttachmentPosition);
    m_lookingDirection = j.value(LOOK_DIRECTION_KEY, m_lookingDirection);
}

bool MainCharacterBehaviourComponent::initialize(EntityContext &ctx) {
    if (!m_cameraName.empty()) {
        if (const auto e = ctx.findEntity(m_cameraName)) {
            m_cameraEntityId = e->m_Id.id();
        }
    }

    return true;
}

void MainCharacterBehaviourComponent::setCameraEntity(const Entity &camera) {
    m_cameraName = camera.m_Name;
    m_cameraEntityId = camera.m_Id.id();
}

void MainCharacterBehaviourComponent::clearCameraEntity() {
    m_cameraName = "";
    m_cameraEntityId = 0;
}

void MainCharacterBehaviourComponent::adjustLookingDirection(glm::vec2 viewChangeAlongScreenAxis) {
    // const glm::vec3 rightHorizontal = glm::normalize(glm::cross(m_lookingDirection, glm::vec3(0, 1, 0)));
    // const glm::vec3 upVertical = glm::normalize(glm::cross(m_lookingDirection, rightHorizontal));
    // const glm::vec3 viewChange = rightHorizontal * viewChangeAlongScreenAxis.x + upVertical * viewChangeAlongScreenAxis.y;
    //
    // m_lookingDirection = glm::normalize(m_lookingDirection + viewChange);

    m_lookingDirection.y += viewChangeAlongScreenAxis.y;
}