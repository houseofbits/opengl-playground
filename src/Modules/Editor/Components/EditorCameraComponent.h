#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/BaseCameraComponent.h"

class EditorCameraComponent  : public Component, public BaseCameraComponent {
    TYPE_DEFINITION(EditorCameraComponent);
public:
    EditorCameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    void moveView(glm::vec3 direction);
};
