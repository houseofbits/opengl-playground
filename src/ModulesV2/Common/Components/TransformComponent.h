#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "../../../CoreV2/API.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class TransformComponent : public Component {
    TYPE_DEFINITION(TransformComponent);

public:
    TransformComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    void resetTransform();
    void setTranslation(glm::vec3 pos);
    void setScale(glm::vec3 scale);
    void setRotation(glm::quat rotation);
    glm::mat4 &getModelMatrix();
    [[nodiscard]] glm::mat4 getInverseModelMatrix() const;
    glm::vec3 getTranslation();
    glm::vec3 getScale();
    [[nodiscard]] glm::vec3 getDirection() const;
    void decomposeModelMatrix(glm::vec3 &, glm::quat &, glm::vec3 &);

    bool m_isTranslationEnabled = true;
    bool m_isRotationEnabled = true;
    bool m_isScalingEnabled = true;

    glm::mat4 m_ModelMatrix;
};
