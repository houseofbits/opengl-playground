#include "PhysicsBodyComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Collision/Shape/StaticCompoundShape.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsBodyComponent::PhysicsBodyComponent() : PhysicsComponent(),
                                               m_BodyType(BODY_TYPE_STATIC),
                                               m_MeshType(MESH_TYPE_TRIANGLE),
                                               m_friction(0.5, 0.5),
                                               m_restitution(0.5),
                                               m_mass(0.0),
                                               m_meshResource(),
                                               m_PhysicsResource(),
                                               m_physicsBodyId() {
}

PhysicsBodyComponent::~PhysicsBodyComponent() = default;

void PhysicsBodyComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = m_BodyType;
    j[SHAPE_KEY] = m_MeshType;
    j[MODEL_KEY] = m_meshResource().m_Path;
    j[RESTITUTION_KEY] = m_restitution;
    j[FRICTION_KEY] = m_friction;
    j[MASS_KEY] = m_mass;
}

void PhysicsBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");

    m_restitution = j.value(RESTITUTION_KEY, m_restitution);
    m_friction = j.value(FRICTION_KEY, m_friction);
    m_mass = j.value(MASS_KEY, m_mass);


    std::string path = j.value(MODEL_KEY, m_meshResource().m_Path);
    resourceManager.request(m_meshResource, path);

    m_BodyType = j.value(TYPE_KEY, m_BodyType);
    m_MeshType = j.value(SHAPE_KEY, m_MeshType);
}

bool PhysicsBodyComponent::isReadyToCreate(EntityContext &ctx) const {
    if (!m_PhysicsResource.isReady()) {
        return false;
    }

    if (!m_meshResource.isReady()) {
        return false;
    }

    if (!ctx.getEntityComponent<TransformComponent>(m_EntityId.id())) {
        return false;
    }

    return true;
}

void PhysicsBodyComponent::createPhysics(EntityContext &ctx) {
    const auto entity = ctx.getEntity(m_EntityId.id());
    if (!entity) {
        return;
    }

    const auto shapeComponents = entity->getAllComponents<PhysicsShapeComponent>();
    if (shapeComponents.empty()) {
        Log::error("PhysicsBodyComponent cannot create: no related shapes found");
    }

    const auto transformComponent = ctx.getEntityComponent<TransformComponent>(m_EntityId.id());
    if (!transformComponent) {
        Log::error("PhysicsBodyComponent cannot create: no transform component found");

        return;
    }


    auto color = (m_BodyType == BODY_TYPE_STATIC)
                     ? glm::vec3{0.5f, 0.5f, 0.5f}
                     : glm::vec3{0, 1, 0};

    JPH::StaticCompoundShapeSettings compoundShapeSettings;
    for (const auto &shapeComponent: shapeComponents) {
        auto shape = shapeComponent->createShape(m_BodyType == BODY_TYPE_DYNAMIC, color);

        if (shape == nullptr) {
            continue;
        }

        compoundShapeSettings.AddShape(
            PhysicsTypeCast::glmToJPH(shapeComponent->m_localPosition),
            PhysicsTypeCast::glmToJPH(shapeComponent->m_localRotation),
            shape
        );
    }

    auto createdShape = compoundShapeSettings.Create();

    if (createdShape.IsEmpty()) {
        Log::write("Failed to create compound shape, isEmpty ", createdShape.GetError());
    }

    if (!createdShape.IsValid()) {
        Log::write("Failed to create compound shape, idValid ", createdShape.GetError());
    }

    auto builder = PhysicsBuilder::newBody(m_PhysicsResource().getSystem())
            .setDebugColor((m_BodyType == BODY_TYPE_STATIC)
                               ? glm::vec3{0.5f, 0.5f, 0.5f}
                               : glm::vec3{0, 1, 0}
            )
            .setEntityReference(m_EntityId.id())
            .setTransform(*transformComponent)
            .setMass(m_mass)
            .setShape(*createdShape.Get().GetPtr())
            .setDamping(0.7, 0.7);

    if (m_BodyType == BODY_TYPE_STATIC) {
        m_physicsBodyId = builder.createStatic()->GetID();
    } else {
        m_physicsBodyId = builder.createDynamic()->GetID();
    }

    m_PhysicsResource().getInterface().SetFriction(m_physicsBodyId, m_friction.x);
    m_PhysicsResource().getInterface().SetRestitution(m_physicsBodyId, m_restitution);
}

bool PhysicsBodyComponent::create(TransformComponent &transform) {
    return false;

    if (!m_meshResource.isReady()) {
        Log::warn("PhysicsBodyComponent::create: Mesh resource not ready");

        return false;
    }

    if (m_MeshType == MESH_TYPE_TRIANGLE && m_BodyType != BODY_TYPE_STATIC) {
        Log::warn("PhysicsBodyComponent::create: Non static body cannot have triangle mesh shape");

        return false;
    }

    if (m_BodyType == BODY_TYPE_DYNAMIC && m_mass < 0.0001) {
        Log::warn("PhysicsBodyComponent::create: Dynamic body should have mass");

        return false;
    }

    auto builder = PhysicsBuilder::newBody(m_PhysicsResource().getSystem())
            .setDebugColor((m_BodyType == BODY_TYPE_STATIC)
                               ? glm::vec3{0.5f, 0.5f, 0.5f}
                               : glm::vec3{0, 1, 0}
            )
            .setEntityReference(m_EntityId.id())
            .setTransform(transform)
            .setMass(80)
            .setDamping(0.7, 0.7);

    if (m_MeshType == MESH_TYPE_TRIANGLE) {
        builder.addTriangleMeshShape(m_meshResource().createTriangleMeshShape(transform.getScale()));
    } else {
        builder.addConvexMeshShape(m_meshResource().createConvexMeshShape(transform.getScale()));
    }

    if (m_BodyType == BODY_TYPE_STATIC) {
        m_physicsBodyId = builder.createStatic()->GetID();
    } else {
        m_physicsBodyId = builder.createDynamic()->GetID();
    }

    m_PhysicsResource().getInterface().SetFriction(m_physicsBodyId, m_friction.x);
    m_PhysicsResource().getInterface().SetRestitution(m_physicsBodyId, m_restitution);

    return true;
}

void PhysicsBodyComponent::release() {
    if (isValid()) {
        m_PhysicsResource().getInterface().RemoveBody(m_physicsBodyId);
        m_PhysicsResource().getInterface().DestroyBody(m_physicsBodyId);
    }
}

void PhysicsBodyComponent::update(TransformComponent &transform, const bool isSimulationEnabled) {
    if (isSimulationEnabled) {
        auto t = m_PhysicsResource().getInterface().GetWorldTransform(m_physicsBodyId);
        glm::vec3 scale = transform.getScale();
        auto m = PhysicsTypeCast::JPHToGlm(t);
        m = glm::scale(m, scale);

        transform.setWorldTransform(m);
    }
}

void PhysicsBodyComponent::wakeUp() {
    if (m_BodyType == BODY_TYPE_DYNAMIC) {
        m_PhysicsResource().getInterface().ActivateBody(m_physicsBodyId);
    }
}

const JPH::Body *PhysicsBodyComponent::getReadableBody() {
    if (m_physicsBodyId.IsInvalid()) {
        return nullptr;
    }

    JPH::BodyLockRead lock(m_PhysicsResource().getLockInterface(), m_physicsBodyId);
    if (lock.Succeeded()) {
        return &lock.GetBody();
    }

    return nullptr;
}

bool PhysicsBodyComponent::isValid() const {
    return !m_physicsBodyId.IsInvalid();
}
