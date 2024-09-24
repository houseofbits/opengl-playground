#include "PhysicsBodyComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Systems/PhysicsBodyProcessingSystem.h"

PhysicsBodyComponent::PhysicsBodyComponent() : Component(),
                                               m_BodyType(BODY_TYPE_STATIC),
                                               m_MeshType(MESH_TYPE_TRIANGLE),
                                               m_friction(0.5, 0.5),
                                               m_restitution(0.5),
                                               m_mass(0.0),
                                               m_meshResource(),
                                               m_rigidBody(nullptr),
                                               m_PhysicsResource() {
}

PhysicsBodyComponent::~PhysicsBodyComponent() {
}

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

void PhysicsBodyComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsBodyProcessingSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

bool PhysicsBodyComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsBodyComponent::create(TransformComponent &transform) {
    if (!m_meshResource().isReady()) {
        return;
    }

    releaseShapes();

    btVector3 localInertia(0, 0, 0);
    auto *motionState = new btDefaultMotionState(PhysicsTypeCast::createBtTransformFromTransformComponent(transform));
    if (m_MeshType == MESH_TYPE_TRIANGLE) {
        btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, m_meshResource().createTriangleMeshShape(transform.getScale()), localInertia);
        m_rigidBody = new btRigidBody(rbInfo);
    } else {
        btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, motionState, m_meshResource().createConvexMeshShape(transform.getScale()), localInertia);
        m_rigidBody = new btRigidBody(rbInfo);
    }
    updateMass();

    m_rigidBody->setDamping(0.4, 0.4);

    m_rigidBody->setUserPointer(new PhysicsUserData(m_EntityId.id()));

    m_PhysicsResource().m_dynamicsWorld->addRigidBody(m_rigidBody);
}

void PhysicsBodyComponent::createMeshShape(TransformComponent &transform) {
    releaseShapes();

    if (m_BodyType == BODY_TYPE_DYNAMIC && m_MeshType == MESH_TYPE_TRIANGLE) {
        Log::warn("PhysicsBodyComponent: Dynamic body cannot have triangle mesh shape");

        return;
    }

    if (m_MeshType == MESH_TYPE_TRIANGLE) {
        m_rigidBody->setCollisionShape(m_meshResource().createTriangleMeshShape(transform.getScale()));
    } else {
        m_rigidBody->setCollisionShape(m_meshResource().createConvexMeshShape(transform.getScale()));
    }
    updateMass();

    m_rigidBody->setFriction(m_friction.x);
    m_rigidBody->setRollingFriction(m_friction.y);
    m_rigidBody->setRestitution(m_restitution);

    m_PhysicsResource().m_dynamicsWorld->addRigidBody(m_rigidBody);
}

void PhysicsBodyComponent::releaseShapes() {
    if (m_rigidBody != nullptr) {
        m_PhysicsResource().m_dynamicsWorld->removeRigidBody(m_rigidBody);
        btCollisionShape *oldShape = m_rigidBody->getCollisionShape();
        delete oldShape;
    }
}

void PhysicsBodyComponent::update(TransformComponent &transform, bool isSimulationEnabled) const {
    if (!isSimulationEnabled) {
        m_rigidBody->setWorldTransform(PhysicsTypeCast::createBtTransformFromTransformComponent(transform));
        m_rigidBody->getMotionState()->setWorldTransform(PhysicsTypeCast::createBtTransformFromTransformComponent(transform));
        m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
        m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
        m_rigidBody->activate();
    } else {
        btTransform t;
        m_rigidBody->getMotionState()->getWorldTransform(t);
        PhysicsTypeCast::applyBtTransformToTransformComponent(transform, t);
    }
}

void PhysicsBodyComponent::updateMass() {
    if (m_mass > 0) {
        if (m_BodyType == BODY_TYPE_STATIC) {
            Log::warn("PhysicsBodyComponent: Static body cannot have mass. Reset static body mass");
            m_mass = 0;
        }

        btVector3 localInertia(0, 0, 0);
        auto *shape = m_rigidBody->getCollisionShape();
        int type = shape->getShapeType();
        if (type == BroadphaseNativeTypes::CONVEX_HULL_SHAPE_PROXYTYPE && m_mass > 0) {
            shape->calculateLocalInertia(m_mass, localInertia);
        }
        m_rigidBody->setMassProps(m_mass, localInertia);
    }
}
