#pragma once

#include "../../../Core/API.h"

class MainCharacterBehaviourComponent : public Component {
TYPE_DEFINITION(MainCharacterBehaviourComponent);

    inline static const std::string ACTIVE_KEY = "active";
    inline static const std::string MLOOK_SPEED_KEY = "mlookSpeed";
    inline static const std::string MOVEMENT_SPEED_KEY = "moveSpeed";
    inline static const std::string CAMERA_NAME_KEY = "cameraName";
    inline static const std::string CAMERA_ATTACHMENT_KEY = "cameraAttachment";
    inline static const std::string LOOK_DIRECTION_KEY = "lookDirection";

public:
    MainCharacterBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool initialize(EntityContext &ctx) override;

    void setCameraEntity(const Entity &camera);

    void clearCameraEntity();

    void adjustLookingDirection(glm::vec2);

    bool m_isActive;
    float m_mouseLookSpeed;
    float m_movementSpeed;
    std::string m_cameraName;
    glm::vec3 m_cameraAttachmentPosition;
    Identity::Type m_cameraEntityId;
    glm::vec3 m_lookingDirection;
};
