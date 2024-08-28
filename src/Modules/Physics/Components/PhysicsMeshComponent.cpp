#include "PhysicsMeshComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsSystem.h"

PhysicsMeshComponent::PhysicsMeshComponent() : Component(),
                                               m_staticBody(nullptr),
                                               m_meshResource(),
                                               m_friction(0.5, 0.5),
                                               m_restitution(0.5),
                                               m_PhysicsResource() {
}

void PhysicsMeshComponent::serialize(nlohmann::json &j) {
    j[MODEL_KEY] = m_meshResource().m_Path;
    j["restitution"] = m_restitution;
    j["friction"] = m_friction;
}

void PhysicsMeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_restitution = j.value("restitution", m_restitution);
    m_friction = j.value("friction", m_friction);

    std::string path = j.value(MODEL_KEY, m_meshResource().m_Path);
    resourceManager.request(m_meshResource, path);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsMeshComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

bool PhysicsMeshComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsMeshComponent::create(TransformComponent &transform) {
    m_staticBody = m_PhysicsResource().m_pxPhysics->createRigidStatic(Types::GLMtoPxTransform(transform.getModelMatrix()));

    createMeshShape(transform);
}

void PhysicsMeshComponent::createMeshShape(TransformComponent &transform) {
    physx::PxVec3 scale = Types::GLMtoPxVec3(transform.getScale());

    auto *vertices = new physx::PxVec3[m_meshResource().m_numVertices];
    for (int i = 0; i < m_meshResource().m_numVertices; ++i) {
        vertices[i] = m_meshResource().m_vertices[i].multiply(scale);
    }

    auto *meshDesc = new physx::PxConvexMeshDesc();
    meshDesc->points.count = m_meshResource().m_numVertices;
    meshDesc->points.stride = sizeof(physx::PxVec3);
    meshDesc->points.data = vertices;
    meshDesc->flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxTolerancesScale toleranceScale;
    physx::PxCookingParams params(toleranceScale);
    physx::PxDefaultMemoryOutputStream writeBuffer;
    physx::PxConvexMeshCookingResult::Enum result;

    bool status = PxCookConvexMesh(params, *meshDesc, writeBuffer, &result);
    if (!status) {
        return;
    }

    physx::PxDefaultMemoryInputData input(writeBuffer.getData(), writeBuffer.getSize());
    physx::PxConvexMesh *convexMesh = m_PhysicsResource().m_pxPhysics->createConvexMesh(input);
    physx::PxMaterial *material = m_PhysicsResource().m_pxPhysics->createMaterial(m_friction.x, m_friction.y, m_restitution);

    auto *shape = m_PhysicsResource().m_pxPhysics->createShape(physx::PxConvexMeshGeometry(convexMesh), *material);

    releaseShapes();

    m_staticBody->attachShape(*shape);
    shape->release();

    delete[] vertices;
    delete meshDesc;
}

void PhysicsMeshComponent::releaseShapes() const {
    physx::PxU32 numShapes = m_staticBody->getNbShapes();
    if (numShapes == 0) {
        return;
    }
    auto **shapes = new physx::PxShape *[numShapes];
    for (auto i = 0; i < m_staticBody->getShapes(shapes, numShapes); ++i) {
        physx::PxShape *shape = shapes[i];
        if (shape != nullptr) {
            m_staticBody->detachShape(*shape);
        }
    }

    delete[] shapes;
}
