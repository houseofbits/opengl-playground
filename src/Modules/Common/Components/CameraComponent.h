#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"

class CameraComponent : public Component {
    TYPE_DEFINITION(CameraComponent);

public:
    enum Type {
        TYPE_FP,
        TYPE_FREE,
    };

    struct TBN {
        glm::vec3 view;
        glm::vec3 up;
        glm::vec3 right;
    };

    inline static std::map<Type, std::string> m_TypeNameMap = {
            {Type::TYPE_FP, "FP"},
            {Type::TYPE_FREE, "FREE"},
    };

    CameraComponent();

    void serialize(nlohmann::json &j) override {
        j["position"] = m_Camera.getPosition();
        j["viewDirection"] = m_Camera.getViewDirection();
        j["upDirection"] = m_Camera.getUpDirection();
        j["isActive"] = m_isActive;
        j["type"] = m_TypeNameMap[m_type];
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        glm::vec3 position = j.value("position", glm::vec3(0, 0, 0));
        glm::vec3 view = j.value("viewDirection", glm::vec3(0, 0, 1));
        glm::vec3 up = j.value("upDirection", glm::vec3(0, 1, 0));

        m_Camera.setPosition(position)
                .setView(view, up)
                .setViewportSize(1024, 768)
                .setFieldOfView(90);

        m_isActive = j.value("isActive", false);
        m_type = getTypeFromName(j.value("type", m_TypeNameMap.begin()->second));
    }

    void registerWithSystems(EntityContext &ctx) override;
    std::string getListName(Entity *e) {
        return e->m_Name + " " + " CAMERA";
    }

    TBN calculateTBN(glm::vec3 viewDirection);

    Camera m_Camera;
    bool m_isActive;
    Type m_type;

private:
    static Type getTypeFromName(const std::string &name) {
        for (const auto &[key, value]: m_TypeNameMap) {
            if (value == name) {
                return key;
            }
        }

        return TYPE_FREE;
    }
};
