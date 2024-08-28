#include "RigidBodyComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsSystem.h"

RigidBodyComponent::RigidBodyComponent() : Component(),
                                           m_pxRigidBody(nullptr),
                                           m_density(100),
                                           m_friction(0.5, 0.5),
                                           m_restitution(0.5),
                                           m_initialTransform(),
                                           m_meshResource(),
                                           m_PhysicsResource() {
}

void RigidBodyComponent::serialize(nlohmann::json &j) {
    j["density"] = m_density;
    j["restitution"] = m_restitution;
    j["friction"] = m_friction;
    if (m_meshResource.isReady()) {
        j[MODEL_KEY] = m_meshResource().m_Path;
    }
}

void RigidBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_density = j.value("density", m_density);
    m_restitution = j.value("restitution", m_restitution);
    m_friction = j.value("friction", m_friction);

    std::string path = j.value(MODEL_KEY, m_meshResource().m_Path);
    resourceManager.request(m_meshResource, path);

    resourceManager.request(m_PhysicsResource, "physics");
}

void RigidBodyComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void RigidBodyComponent::updateBodyParameters() {
    if (m_pxRigidBody != nullptr) {
        physx::PxRigidBodyExt::updateMassAndInertia(*m_pxRigidBody, m_density);

        //        physx::PxU32 nShapes = m_pxRigidBody->getNbShapes();
        //        auto** shapes = new physx::PxShape*[nShapes];
        //
        //        m_pxRigidBody->getShapes(shapes, nShapes);
        //        while (nShapes--)
        //        {
        //            shapes[nShapes]->getNbMaterials();
        //        }
        //        delete[] shapes;
    }
}

void RigidBodyComponent::create(TransformComponent &transform) {
    m_pxRigidBody = m_PhysicsResource().m_pxPhysics->createRigidDynamic(Types::GLMtoPxTransform(transform.getModelMatrix()));
    m_initialTransform = transform.getModelMatrix();

    createMeshShape(transform);

    physx::PxRigidBodyExt::updateMassAndInertia(*m_pxRigidBody, m_density);
    m_pxRigidBody->setSleepThreshold(0.005f);
    m_pxRigidBody->setAngularDamping(0.1f);
}

void RigidBodyComponent::createMeshShape(TransformComponent &transform) {
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

    shape->setContactOffset(0.002f);
    shape->setRestOffset(0.0001f);

    releaseShapes();

    m_pxRigidBody->attachShape(*shape);
    shape->release();

    delete[] vertices;
    delete meshDesc;
}

void RigidBodyComponent::releaseShapes() const {
    physx::PxU32 numShapes = m_pxRigidBody->getNbShapes();
    if (numShapes == 0) {
        return;
    }
    auto **shapes = new physx::PxShape *[numShapes];
    for (auto i = 0; i < m_pxRigidBody->getShapes(shapes, numShapes); ++i) {
        physx::PxShape *shape = shapes[i];
        if (shape != nullptr) {
            m_pxRigidBody->detachShape(*shape);
        }
    }

    delete[] shapes;
}

bool RigidBodyComponent::isReady() {
    return m_PhysicsResource.isReady() && m_meshResource.isReady();
}
