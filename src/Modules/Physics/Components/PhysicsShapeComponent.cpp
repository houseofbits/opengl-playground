#include "PhysicsShapeComponent.h"
#include "../../../Core/Helper/Math.h"
#include <glm/gtx/quaternion.hpp>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/PhysicsShapeUserData.h"

PhysicsShapeComponent::PhysicsShapeComponent() : Component(), m_doMergeMeshNodes(true) {
}

void PhysicsShapeComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = m_type;
    j[POS_KEY] = m_localPosition;
    j[ROT_KEY] = m_localRotation;
    if (m_type == TYPE_BOX) {
        j[BOX_SIZE_KEY] = m_boxSize;
    }
    if (m_type == TYPE_SPHERE) {
        j[SPHERE_RADIUS_KEY] = m_sphereRadius;
    }
    if (m_type == TYPE_MESH) {
        j[MESH_FILE_KEY] = m_meshResource().m_Path;
        j[MESH_SCALE_KEY] = m_meshScale;
        j[MERGE_MESH_KEY] = m_doMergeMeshNodes;
    }
}

void PhysicsShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_type = j.value(TYPE_KEY, m_type);
    m_localPosition = j.value(POS_KEY, m_localPosition);
    m_localRotation = j.value(ROT_KEY, m_localRotation);

    if (m_type == TYPE_MESH) {
        std::string path = j.value(MESH_FILE_KEY, m_meshResource().m_Path);
        resourceManager.request(m_meshResource, path);
        m_meshScale = j.value(MESH_SCALE_KEY, m_meshScale);
        m_doMergeMeshNodes = j.value(MERGE_MESH_KEY, m_doMergeMeshNodes);
    }
    if (m_type == TYPE_BOX) {
        m_boxSize = j.value(BOX_SIZE_KEY, m_boxSize);
    }
    if (m_type == TYPE_SPHERE) {
        m_sphereRadius = j.value(SPHERE_RADIUS_KEY, m_sphereRadius);
    }
}

glm::mat4 PhysicsShapeComponent::getWorldTransform(const glm::mat4 &parentTransform) const {
    glm::mat4 localTransform = glm::mat4(1.0f);
    localTransform = glm::toMat4(m_localRotation);

    auto scale = glm::vec3(1.0);
    if (m_type == TYPE_BOX) {
        scale = m_boxSize;
    }
    if (m_type == TYPE_SPHERE) {
        scale = glm::vec3(m_sphereRadius);
    }
    if (m_type == TYPE_MESH) {
        scale = m_meshScale;
    }

    localTransform = glm::scale(localTransform, scale);
    localTransform[3] = glm::vec4(m_localPosition, 1.0);

    return Math::rescaleMatrix(parentTransform) * localTransform;
}

JPH::Shape *PhysicsShapeComponent::createMergedShape(bool isDynamic, const glm::vec3 debugColor) {
    const auto material = new JPH::PhysicsMaterialSimple(
        "Material", JPH::Color(static_cast<unsigned short>(255.0f * debugColor.x),
                               static_cast<unsigned short>(255.0f * debugColor.y),
                               static_cast<unsigned short>(255.0f * debugColor.z)));

    if (m_type == TYPE_MESH) {
        if (isDynamic) {
            const auto shapeSettings = new JPH::ConvexHullShapeSettings(
                m_meshResource().getAllMeshVertices(m_meshScale),
                JPH::cDefaultConvexRadius, material);

            const auto shape = shapeSettings->Create().Get().GetPtr();
            setUserData(shape, m_Name);

            return shape;
        } else {
            JPH::PhysicsMaterialList materials;
            materials.push_back(material);

            const auto shapeSettings = new JPH::MeshShapeSettings(
                m_meshResource().getAllMeshTriangles(m_meshScale),
                std::move(materials));

            const auto shape = shapeSettings->Create().Get().GetPtr();
            setUserData(shape, m_Name);

            return shape;
        }
    }
    if (m_type == TYPE_BOX) {
        const auto shape = new JPH::BoxShape(PhysicsTypeCast::glmToJPH(m_boxSize * 0.5f),
                                             0.0f,
                                             material);
        setUserData(shape, m_Name);

        return shape;
    }
    if (m_type == TYPE_SPHERE) {
        auto sensorShape = new JPH::SphereShapeSettings(2.0f);


        const auto shape = new JPH::SphereShape(m_sphereRadius, material);
        setUserData(shape, m_Name);

        return shape;
    }

    return nullptr;
}

void PhysicsShapeComponent::addCompoundShape(JPH::StaticCompoundShapeSettings &compound, const bool isDynamic,
                                             const glm::vec3 debugColor) {
    if (m_type == TYPE_MESH && m_doMergeMeshNodes == false) {
        const auto material = new JPH::PhysicsMaterialSimple(
            "Material", JPH::Color(static_cast<unsigned short>(255.0f * debugColor.x),
                                   static_cast<unsigned short>(255.0f * debugColor.y),
                                   static_cast<unsigned short>(255.0f * debugColor.z)));

        for (const auto &meshNode: m_meshResource().m_model.nodes) {
            const auto vertices = m_meshResource().getMeshNodeVertices(meshNode, m_meshScale);

            const auto shapeSettings = new JPH::ConvexHullShapeSettings(
                vertices,
                JPH::cDefaultConvexRadius, material);

            const auto shape = shapeSettings->Create().Get().GetPtr();
            setUserData(shape, meshNode.name);

            compound.AddShape(
                JPH::Vec3::sZero(),
                JPH::Quat::sIdentity(),
                shape
            );
        }
    } else {
        const auto shape = createMergedShape(isDynamic, debugColor);

        if (shape == nullptr) {
            return;
        }

        compound.AddShape(
            PhysicsTypeCast::glmToJPH(m_localPosition),
            PhysicsTypeCast::glmToJPH(m_localRotation),
            shape
        );
    }
}

void PhysicsShapeComponent::setUserData(JPH::Shape *shape, const std::string &name) const {
    auto *userData = new PhysicsShapeUserData(m_Id.id(), name);
    shape->SetUserData(reinterpret_cast<unsigned long>(userData));
}
