#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "../../../Core/API.h"
//#include <PhysX/PxPhysics.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class TransformComponent : public Component {
    TYPE_DEFINITION(TransformComponent);

public:
    inline static const std::string TRANSLATION_KEY = "translation";
    inline static const std::string ROTATION_KEY = "rotation";
    inline static const std::string SCALE_KEY = "scale";
    inline static const std::string ALLOW_TRANSLATION_KEY = "allowTranslation";
    inline static const std::string ALLOW_ROTATION_KEY = "allowRotation";
    inline static const std::string ALLOW_SCALING_KEY = "allowScaling";

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
    glm::quat getRotation();
    [[nodiscard]] glm::vec3 getDirection() const;
    void decomposeModelMatrix(glm::vec3 &, glm::quat &, glm::vec3 &);
//    void setFromPxTransform(const physx::PxTransform&);

    glm::vec3 getInitialTranslation();
    glm::vec3 getInitialScale();
    glm::quat getInitialRotation();

    bool m_isTranslationEnabled;
    bool m_isRotationEnabled;
    bool m_isScalingEnabled;
    glm::mat4 m_transform;
    glm::mat4 m_initialTransform;
};
