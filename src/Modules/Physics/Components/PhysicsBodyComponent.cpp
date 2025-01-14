#include "PhysicsBodyComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Systems/PhysicsBodyProcessingSystem.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Collision/Shape/ConvexHullShape.h"
#include "Jolt/Physics/Collision/PhysicsMaterialSimple.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

PhysicsBodyComponent::PhysicsBodyComponent() : Component(),
                                               m_BodyType(BODY_TYPE_STATIC),
                                               m_MeshType(MESH_TYPE_TRIANGLE),
                                               m_friction(0.5, 0.5),
                                               m_restitution(0.5),
                                               m_mass(0.0),
                                               m_meshResource(),
                                               m_physicsBody(nullptr),
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

    if (m_MeshType == MESH_TYPE_TRIANGLE && m_BodyType != BODY_TYPE_STATIC) {
        Log::warn("PhysicsBodyComponent::create: Non static body cannot have triangle mesh shape");

        return;
    }

    releaseShapes();

    JPH::PhysicsMaterialList materials;

    JPH::ShapeSettings *shapeSettings = nullptr;
    if (m_MeshType == MESH_TYPE_TRIANGLE) {
        materials.push_back(new JPH::PhysicsMaterialSimple("Material", JPH::Color::sGrey));
        shapeSettings = new JPH::MeshShapeSettings(m_meshResource().createTriangleMeshShape(transform.getScale()),
                                                   std::move(materials));
    } else {
        shapeSettings = new JPH::ConvexHullShapeSettings(m_meshResource().createConvexMeshShape(transform.getScale()));
    }

    JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
    JPH::ObjectLayer layer = Layers::MOVING;
    if (m_BodyType == BODY_TYPE_STATIC) {
        motionType = JPH::EMotionType::Static;
        layer = Layers::NON_MOVING;
    }

    auto settings = JPH::BodyCreationSettings(
            shapeSettings,
            PhysicsTypeCast::glmToJPH(transform.getTranslation()),
            PhysicsTypeCast::glmToJPH(transform.getRotation()),
            motionType,
            layer);

    settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
    settings.mMassPropertiesOverride.mMass = m_mass;
    settings.mLinearDamping = 0.7;
    settings.mAngularDamping = 0.7;

    m_physicsBody = m_PhysicsResource().getInterface().CreateBody(settings);

    m_physicsBody->SetFriction(m_friction.x);
    m_physicsBody->SetRestitution(m_restitution);

    auto *userData = new PhysicsUserData(m_EntityId.id());
    m_physicsBody->SetUserData(reinterpret_cast<unsigned long>(userData));

    m_PhysicsResource().getInterface().AddBody(m_physicsBody->GetID(), JPH::EActivation::Activate);
}

void PhysicsBodyComponent::createMeshShape(TransformComponent &transform) {
    releaseShapes();

    if (m_BodyType == BODY_TYPE_DYNAMIC && m_MeshType == MESH_TYPE_TRIANGLE) {
        Log::warn("PhysicsBodyComponent: Dynamic body cannot have triangle mesh shape");

        return;
    }

//    if (m_MeshType == MESH_TYPE_TRIANGLE) {
//        m_rigidBody->setCollisionShape(m_meshResource().createTriangleMeshShape(transform.getScale()));
//    } else {
//        m_rigidBody->setCollisionShape(m_meshResource().createConvexMeshShape(transform.getScale()));
//    }
//    updateMass();
//
//    m_rigidBody->setFriction(m_friction.x);
//    m_rigidBody->setRollingFriction(m_friction.y);
//    m_rigidBody->setRestitution(m_restitution);
//
//    m_PhysicsResource().m_dynamicsWorld->addRigidBody(m_rigidBody);
}

void PhysicsBodyComponent::releaseShapes() {
//    if (m_rigidBody != nullptr) {
//        m_PhysicsResource().m_dynamicsWorld->removeRigidBody(m_rigidBody);
//        btCollisionShape *oldShape = m_rigidBody->getCollisionShape();
//        delete oldShape;
//    }
}

void PhysicsBodyComponent::update(TransformComponent &transform, bool isSimulationEnabled) {
    if (isSimulationEnabled) {
        auto t = m_physicsBody->GetWorldTransform();
        PhysicsTypeCast::applyJPHMat44ToTransformComponent(transform, t);
    } else {
        m_PhysicsResource().getInterface().SetPositionAndRotation(m_physicsBody->GetID(),
                                                                  PhysicsTypeCast::glmToJPH(transform.getTranslation()),
                                                                  PhysicsTypeCast::glmToJPH(transform.getRotation()),
                                                                  JPH::EActivation::DontActivate);
    }
}

void PhysicsBodyComponent::updateMass() {
    if (m_mass > 0) {
//        if (m_BodyType == BODY_TYPE_STATIC) {
//            Log::warn("PhysicsBodyComponent: Static body cannot have mass. Reset static body mass");
//            m_mass = 0;
//        }
//
//        btVector3 localInertia(0, 0, 0);
//        auto *shape = m_rigidBody->getCollisionShape();
//        int type = shape->getShapeType();
//        if (type == BroadphaseNativeTypes::CONVEX_HULL_SHAPE_PROXYTYPE && m_mass > 0) {
//            shape->calculateLocalInertia(m_mass, localInertia);
//        }
//        m_rigidBody->setMassProps(m_mass, localInertia);
    }
}

bool PhysicsBodyComponent::isCreated() const {
    return m_physicsBody != nullptr;
}

void PhysicsBodyComponent::wakeUp() {
    if (m_BodyType == BODY_TYPE_DYNAMIC) {
        m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());
    }
}
