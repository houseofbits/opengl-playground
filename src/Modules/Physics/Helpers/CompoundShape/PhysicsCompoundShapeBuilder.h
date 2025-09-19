#pragma once

#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include "../../../../Core/Json/JsonTypes.h"

class PhysicsBaseShape {
public:
    virtual ~PhysicsBaseShape() = default;

    enum Type {
        TYPE_BOX = 1,
        TYPE_SPHERE = 2,
    };

    std::string m_name;
    glm::vec3 m_position{0.0f};

    virtual void serialize(nlohmann::json &j) {
        j["name"] = m_name;
        j["position"] = m_position;
    }

    virtual void deserialize(const nlohmann::json &j) {
        m_name = j.value("name", "");
        m_position = j.value("position", glm::vec3(0.0f));
    }
};

class PhysicsBoxShape : public PhysicsBaseShape {
public:
    glm::vec3 m_size{1.0f};
    glm::quat m_rotation{1, 0, 0, 0};

    void serialize(nlohmann::json &j) override {
        PhysicsBaseShape::serialize(j);
        j["type"] = TYPE_BOX;
        j["size"] = m_size;
        j["rotation"] = m_rotation;
    }

    void deserialize(const nlohmann::json &j) override {
        PhysicsBaseShape::deserialize(j);
        m_size = j.value("size", glm::vec3(1.0f));
        m_rotation = j.value("rotation", glm::quat(1, 0, 0, 0));
    }
};

class PhysicsSphereShape : public PhysicsBaseShape {
public:
    float m_radius{1.0f};

    void serialize(nlohmann::json &j) override {
        PhysicsBaseShape::serialize(j);
        j["type"] = TYPE_SPHERE;
        j["radius"] = m_radius;
    }

    void deserialize(const nlohmann::json &j) override {
        PhysicsBaseShape::deserialize(j);
        m_radius = j.value("radius", 1.0f);
    }
};

class PhysicsCompoundShapeBuilder {
public:
    PhysicsCompoundShapeBuilder();

    void serialize(nlohmann::json &j);

    void deserialize(const nlohmann::json &j);

    std::vector<PhysicsBaseShape *> m_shapes;

private:
    static PhysicsBaseShape* createShape(PhysicsBaseShape::Type);
};
