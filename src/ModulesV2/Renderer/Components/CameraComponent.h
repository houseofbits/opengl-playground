#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Camera/Camera.h"

class CameraComponent : public Component {
public:
    CameraComponent();

    void serialize(nlohmann::json &j) override {
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        glm::vec3 position = j.value("position", glm::vec3(0, 0, 0));
        float horizontalAngle = j.value("horizontalAngle", 0.0f);
        float verticalAngle = j.value("verticalAngle", 0.0f);

        m_Camera.setPosition(position)
                .setAngles(horizontalAngle, verticalAngle)
                .setViewportSize(1024, 768)
                .setFieldOfView(90);

        m_isActive = true;
    }

    void registerWithSystems(EntityContext &ctx) override;

    Camera m_Camera;
    bool m_isActive;
};
