#pragma once

#include "../../../CoreV2/API.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class TransformComponent : public Component {
public:
    TransformComponent();

    void serialize(nlohmann::json &j) override {
        glm::vec3 t;
        glm::quat r;
        glm::vec3 s;

        decomposeModelMatrix(t, r, s);

        j["translation"] = t;
        j["rotation"] = r;
        j["scale"] = s;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        m_Translation = j.value("translation", m_Translation);
        m_Rotation = j.value("rotation", m_Rotation);
        m_Scale = j.value("scale", m_Scale);

        m_isTranslationEnabled = j.value("allowTranslation", m_isTranslationEnabled);
        m_isRotationEnabled = j.value("allowRotation", m_isRotationEnabled);
        m_isScalingEnabled = j.value("allowScaling", m_isScalingEnabled);

        setTranslation(m_Translation);
        setRotation(m_Rotation);
        setScale(m_Scale);
    }

    void registerWithSystems(EntityContext &ctx) override;

    void resetTransform() {
        m_Translation = glm::vec3(0, 0, 0);
        m_Scale = glm::vec3(1, 1, 1);
        m_Rotation = glm::vec3(0, 0, 0);
        m_ModelMatrix = glm::mat4(1.0);
    }
    void setTranslation(glm::vec3 pos) {
        if (m_isTranslationEnabled) {
            m_Translation = pos;
            m_ModelMatrix = glm::translate(m_ModelMatrix, m_Translation);
        }
    }
    void setScale(glm::vec3 scale) {
        if (m_isScalingEnabled) {
            m_Scale = scale;
            m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
        }
    }
    void setRotation(glm::quat rotation) {
        if (m_isRotationEnabled) {
            m_Rotation = rotation;

            glm::mat4 rotationMatrix = glm::mat4_cast(m_Rotation);
            m_ModelMatrix *= rotationMatrix;
        }
    }

    glm::mat4 &getModelMatrix() {
        return m_ModelMatrix;
    }

    [[nodiscard]] glm::mat4 getInverseModelMatrix() const {
        return glm::inverse(m_ModelMatrix);
    }

    void decomposeModelMatrix(glm::vec3 &, glm::quat &, glm::vec3 &);

    glm::vec3 m_Translation;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    bool m_isTranslationEnabled = true;
    bool m_isRotationEnabled = true;
    bool m_isScalingEnabled = true;

    glm::mat4 m_ModelMatrix;
};
