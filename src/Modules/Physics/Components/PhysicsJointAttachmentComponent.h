#pragma once

#include "../../../Core/API.h"

class PhysicsJointAttachmentComponent : public Component {
    TYPE_DEFINITION(PhysicsJointAttachmentComponent);

    inline static const std::string ATTACHMENT_POSITION_KEY = "attachmentPosition";
    inline static const std::string ATTACHMENT_ROTATION_KEY = "attachmentRotation";

public:
    PhysicsJointAttachmentComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    glm::mat4 m_localAttachmentMatrix;
};