#include "PhysicsBodyComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsBodyProcessingSystem.h"

PhysicsBodyComponent::PhysicsBodyComponent() : Component(),
                                               m_BodyType(BODY_TYPE_STATIC),
                                               m_MeshType(MESH_TYPE_TRIANGLE),
                                               m_friction(0.5, 0.5),
                                               m_restitution(0.5),
                                               m_density(1.0),
                                               m_meshResource(),
                                               m_PhysicsResource(),
                                               m_pxRigidActor(nullptr) {
}

PhysicsBodyComponent::~PhysicsBodyComponent() {
    if (m_pxRigidActor != nullptr) {
        delete (PhysicsActorUserData *) m_pxRigidActor->userData;
        releaseShapes();
        m_pxRigidActor->release();
    }
}

void PhysicsBodyComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = m_BodyType;
    j[SHAPE_KEY] = m_MeshType;
    j[MODEL_KEY] = m_meshResource().m_Path;
    j[RESTITUTION_KEY] = m_restitution;
    j[FRICTION_KEY] = m_friction;
    j[DENSITY_KEY] = m_density;
}

void PhysicsBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_restitution = j.value(RESTITUTION_KEY, m_restitution);
    m_friction = j.value(FRICTION_KEY, m_friction);
    m_density = j.value(DENSITY_KEY, m_density);

    std::string path = j.value(MODEL_KEY, m_meshResource().m_Path);
    resourceManager.request(m_meshResource, path);

    resourceManager.request(m_PhysicsResource, "physics");

    m_BodyType = j.value(TYPE_KEY, m_BodyType);
    m_MeshType = j.value(SHAPE_KEY, m_MeshType);
}

void PhysicsBodyComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsBodyProcessingSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

bool PhysicsBodyComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsBodyComponent::create(TransformComponent &transform) {
    if (m_pxRigidActor != nullptr) {
        m_PhysicsResource().m_pxScene->removeActor(*m_pxRigidActor);
        delete (PhysicsActorUserData *) m_pxRigidActor->userData;
        releaseShapes();
        m_pxRigidActor->release();
    }

    if (m_BodyType == BODY_TYPE_STATIC) {
        m_pxRigidActor = m_PhysicsResource().m_pxPhysics->createRigidStatic(Types::GLMtoPxTransform(transform.getModelMatrix()));
    } else if (m_BodyType == BODY_TYPE_DYNAMIC) {
        auto *rigidBody = m_PhysicsResource().m_pxPhysics->createRigidDynamic(Types::GLMtoPxTransform(transform.getModelMatrix()));

        rigidBody->setSleepThreshold(0.005f);
        rigidBody->setAngularDamping(0.1f);

        physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, m_density);

        m_pxRigidActor = rigidBody;
    }

    m_pxRigidActor->userData = new PhysicsActorUserData(m_EntityId.id());

    createMeshShape(transform);
}

void PhysicsBodyComponent::createMeshShape(TransformComponent &transform) {
    physx::PxShape *shape = nullptr;
    if (m_MeshType == MESH_TYPE_CONVEX) {
        shape = createConvexMeshShape(transform.getScale());
    } else if (m_MeshType == MESH_TYPE_TRIANGLE) {
        shape = createTriangleMeshShape(transform.getScale());
    }

    if (shape == nullptr) {
        return;
    }

    releaseShapes();
    m_pxRigidActor->attachShape(*shape);

    shape->release();
}

physx::PxShape *PhysicsBodyComponent::createConvexMeshShape(glm::vec3 scale) {
    physx::PxVec3 pxScale = Types::GLMtoPxVec3(scale);

    auto *vertices = new physx::PxVec3[m_meshResource().m_numVertices];
    for (int i = 0; i < m_meshResource().m_numVertices; ++i) {
        vertices[i] = m_meshResource().m_vertices[i].multiply(pxScale);
    }

    physx::PxConvexMeshDesc meshDesc;
    meshDesc.points.count = m_meshResource().m_numVertices;
    meshDesc.points.stride = sizeof(physx::PxVec3);
    meshDesc.points.data = vertices;
    meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxTolerancesScale toleranceScale;
    physx::PxCookingParams params(toleranceScale);
    physx::PxDefaultMemoryOutputStream writeBuffer;
    physx::PxConvexMeshCookingResult::Enum result;

    bool status = PxCookConvexMesh(params, meshDesc, writeBuffer, &result);

    delete[] vertices;

    if (!status) {
        return nullptr;
    }

    physx::PxDefaultMemoryInputData input(writeBuffer.getData(), writeBuffer.getSize());
    physx::PxConvexMesh *convexMesh = m_PhysicsResource().m_pxPhysics->createConvexMesh(input);
    physx::PxMaterial *material = m_PhysicsResource().m_pxPhysics->createMaterial(m_friction.x, m_friction.y, m_restitution);

    return m_PhysicsResource().m_pxPhysics->createShape(physx::PxConvexMeshGeometry(convexMesh), *material);
}

physx::PxShape *PhysicsBodyComponent::createTriangleMeshShape(glm::vec3 scale) {
    physx::PxVec3 pxScale = Types::GLMtoPxVec3(scale);

    auto *vertices = new physx::PxVec3[m_meshResource().m_numVertices];
    for (int i = 0; i < m_meshResource().m_numVertices; ++i) {
        vertices[i] = m_meshResource().m_vertices[i].multiply(pxScale);
    }

    physx::PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = m_meshResource().m_numVertices;
    meshDesc.points.stride = sizeof(physx::PxVec3);
    meshDesc.points.data = vertices;
    meshDesc.triangles.count = m_meshResource().m_numIndices / 3;
    meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
    meshDesc.triangles.data = m_meshResource().m_indices;

    physx::PxTolerancesScale toleranceScale;
    physx::PxCookingParams params(toleranceScale);
    physx::PxDefaultMemoryOutputStream writeBuffer;
    physx::PxTriangleMeshCookingResult::Enum result;

    bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);

    delete[] vertices;

    if (!status) {
        return nullptr;
    }

    physx::PxDefaultMemoryInputData input(writeBuffer.getData(), writeBuffer.getSize());
    physx::PxTriangleMesh *triangleMesh = m_PhysicsResource().m_pxPhysics->createTriangleMesh(input);
    physx::PxMaterial *material = m_PhysicsResource().m_pxPhysics->createMaterial(m_friction.x, m_friction.y, m_restitution);

    return m_PhysicsResource().m_pxPhysics->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *material);
}

void PhysicsBodyComponent::releaseShapes() const {
    physx::PxU32 numShapes = m_pxRigidActor->getNbShapes();
    if (numShapes == 0) {
        return;
    }
    auto **shapes = new physx::PxShape *[numShapes];
    for (auto i = 0; i < m_pxRigidActor->getShapes(shapes, numShapes); ++i) {
        physx::PxShape *shape = shapes[i];
        if (shape != nullptr) {
            m_pxRigidActor->detachShape(*shape);
        }
    }

    delete[] shapes;
}

void PhysicsBodyComponent::update(TransformComponent &transform, bool isSimulationEnabled) const {
    if (m_BodyType == BODY_TYPE_STATIC || !isSimulationEnabled) {
        m_pxRigidActor->setGlobalPose(Types::GLMtoPxTransform(transform.getModelMatrix()));
    } else if (m_BodyType == BODY_TYPE_DYNAMIC) {
        transform.setFromPxTransform(m_pxRigidActor->getGlobalPose());
    }
}
