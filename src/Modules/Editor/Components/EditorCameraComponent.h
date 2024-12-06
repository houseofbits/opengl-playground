#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"

class EditorCameraComponent  : public Component {
    TYPE_DEFINITION(EditorCameraComponent);
public:
    EditorCameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void registerWithSystems(EntityContext &ctx) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    void moveView(glm::vec3 direction);

    Camera m_Camera;
    bool m_isActive;
};
