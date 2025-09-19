#include "PhysicsCompoundShapeBuilder.h"

PhysicsCompoundShapeBuilder::PhysicsCompoundShapeBuilder() {
}

void PhysicsCompoundShapeBuilder::serialize(nlohmann::json &j) {
    for (auto shape: m_shapes) {
        nlohmann::json shapeJson;
        shape->serialize(shapeJson);
        j.push_back(shapeJson);
    }
}

void PhysicsCompoundShapeBuilder::deserialize(const nlohmann::json &j) {
    for (const auto &shapeJson: j.items()) {
        if (shapeJson.value().contains("type")) {
            auto type = shapeJson.value().at("type");

            auto shape = createShape(type);
            if (shape) {
                shape->deserialize(shapeJson.value());
                m_shapes.push_back(shape);
            }
        }
    }
}

PhysicsBaseShape *PhysicsCompoundShapeBuilder::createShape(const PhysicsBaseShape::Type type) {
    switch (type) {
        case PhysicsBaseShape::TYPE_BOX:
            return new PhysicsBoxShape();
        case PhysicsBaseShape::TYPE_SPHERE:
            return new PhysicsSphereShape();
    }
    return nullptr;
}
