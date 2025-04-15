#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsMeshResource.h"

class PhysicsShapeComponent : public Component {
    TYPE_DEFINITION(PhysicsShapeComponent);

    inline static const std::string TYPE_KEY = "type";
    inline static const std::string POS_KEY = "position";
    inline static const std::string ROT_KEY = "rotation";
    inline static const std::string BOX_SIZE_KEY = "boxSize";
    inline static const std::string MESH_SACALE_KEY = "meshScale";
    inline static const std::string SPHERE_RADIUS_KEY = "sphereRadius";
    inline static const std::string MESH_FILE_KEY = "mesh";

public:
    PhysicsShapeComponent();

    enum Type {
        TYPE_UNDEFINED = 0,
        TYPE_BOX,
        TYPE_SPHERE,
        TYPE_MESH,
    };

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    JPH::Shape* createShape(bool isDynamic, glm::vec3 debugColor = {0.5, 0.5, 0.5});

    [[nodiscard]] glm::mat4 getWorldTransform(const glm::mat4& parentTransform) const;

    Type m_type{TYPE_UNDEFINED};
    glm::vec3 m_localPosition{0.0};
    glm::quat m_localRotation{1, 0, 0, 0};
    float m_sphereRadius{1.0};
    glm::vec3 m_boxSize{1.0};
    glm::vec3 m_meshScale{1.0};
    ResourceHandle<PhysicsMeshResource> m_meshResource;

private:
    void setUserData(JPH::Shape* shape) const;
};
