#pragma once

#include "../../../CoreV2/API.h"
#include <glm/vec3.hpp>

class TransformComponent : public Component {
public:
    TransformComponent();

    void serialize(nlohmann::json &j) override {
        j["translation"] = m_Translation;
        j["rotation"] = m_Rotation;
        j["scale"] = m_Scale;
    }

    void deserialize(const nlohmann::json &j) override {
        m_Translation = j.value("translation", m_Translation);
        m_Rotation = j.value("rotation", m_Rotation);
        m_Scale = j.value("scale", m_Scale);
        m_isTranslationEnabled = j.value("allowTranslation", m_isTranslationEnabled);
        m_isRotationEnabled = j.value("allowRotation", m_isRotationEnabled);
        m_isScalingEnabled = j.value("allowScaling", m_isScalingEnabled);
    }

    void registerWithSystems(EntityContext& ctx) override;

    glm::vec3 m_Translation;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    bool m_isTranslationEnabled = true;
    bool m_isRotationEnabled = true;
    bool m_isScalingEnabled = true;
};
