#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"

class CameraComponent : public Component {
    TYPE_DEFINITION(CameraComponent);

public:
    CameraComponent();

    void serialize(nlohmann::json &j) override {
        j["position"] = m_Camera.getPosition();
        j["viewDirection"] = m_Camera.getViewDirection();
        j["upDirection"] = m_Camera.getUpDirection();
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        glm::vec3 position = j.value("position", glm::vec3(0, 0, 0));
        glm::vec3 view = j.value("viewDirection", glm::vec3(0, 0, 1));
        glm::vec3 up = j.value("upDirection", glm::vec3(0, 1, 0));

        m_Camera.setPosition(position)
                .setView(view, up)
                .setViewportSize(1024, 768)
                .setFieldOfView(90);

        m_isActive = true;
    }

    void registerWithSystems(EntityContext &ctx) override;
    std::string getListName(Entity* e) {
        return e->m_Name + " " + " CAMERA";
    }

    Camera m_Camera;
    bool m_isActive;
};
