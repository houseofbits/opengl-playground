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

        if (m_isTranslationEnabled) {
            j["translation"] = t;
        }
        if (m_isRotationEnabled) {
            j["rotation"] = r;
        }
        if (m_isScalingEnabled) {
            j["scale"] = s;
        }
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        glm::vec3 t = j.value("translation", glm::vec3(0, 0, 0));
        glm::quat r = j.value("rotation", glm::quat(1, 0, 0, 0));
        glm::vec3 s = j.value("scale", glm::vec3(1, 1, 1));

        m_isTranslationEnabled = j.value("allowTranslation", m_isTranslationEnabled);
        m_isRotationEnabled = j.value("allowRotation", m_isRotationEnabled);
        m_isScalingEnabled = j.value("allowScaling", m_isScalingEnabled);

        if (m_isTranslationEnabled) {
            setTranslation(t);
        }
        if (m_isRotationEnabled) {
            setRotation(r);
        }
        if (m_isScalingEnabled) {
            setScale(s);
        }
    }

    void registerWithSystems(EntityContext &ctx) override;

    void resetTransform() {
        m_ModelMatrix = glm::mat4(1.0);
    }
    void setTranslation(glm::vec3 pos) {
        if (m_isTranslationEnabled) {
            m_ModelMatrix = glm::translate(m_ModelMatrix, pos);
        }
    }
    void setScale(glm::vec3 scale) {
        if (m_isScalingEnabled) {
            m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
        }
    }
    void setRotation(glm::quat rotation) {
        if (m_isRotationEnabled) {
            glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
            m_ModelMatrix *= rotationMatrix;
        }
    }

    glm::mat4 &getModelMatrix() {
        return m_ModelMatrix;
    }

    [[nodiscard]] glm::mat4 getInverseModelMatrix() const {
        return glm::inverse(m_ModelMatrix);
    }

    glm::vec3 getTranslation() {
        return m_ModelMatrix[3];
    }

    glm::vec3 getScale() {
        glm::vec3 scale;
        scale[0] = glm::length(glm::vec3(m_ModelMatrix[0]));
        scale[1] = glm::length(glm::vec3(m_ModelMatrix[1]));
        scale[2] = glm::length(glm::vec3(m_ModelMatrix[2]));
        
        return scale;
    }

    [[nodiscard]] glm::vec3 getDirection() const {
        glm::quat rotation = glm::quat_cast(m_ModelMatrix);

        return rotation * glm::vec3(0, 0, 1);
    }

    void decomposeModelMatrix(glm::vec3 &, glm::quat &, glm::vec3 &);

    bool m_isTranslationEnabled = true;
    bool m_isRotationEnabled = true;
    bool m_isScalingEnabled = true;

    glm::mat4 m_ModelMatrix;
};
