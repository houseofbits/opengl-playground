#include "PhysicsBodyComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Collision/Shape/StaticCompoundShape.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsBodyComponent::PhysicsBodyComponent() : PhysicsComponent(),
                                               m_BodyType(BODY_TYPE_STATIC),
                                               m_isSensor(false),
                                               m_excludeSensorFromActionHit(false),
                                               m_friction(0.5, 0.5),
                                               m_damping(0.7, 0.7),
                                               m_restitution(0.5),
                                               m_mass(0.0),
                                               m_gravityFactor(1.0),
                                               m_PhysicsResource(),
                                               m_physicsBodyId() {
}

PhysicsBodyComponent::~PhysicsBodyComponent() = default;

void PhysicsBodyComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = m_BodyType;
    j[RESTITUTION_KEY] = m_restitution;
    j[FRICTION_KEY] = m_friction;
    j[DAMPING_KEY] = m_damping;
    j[MASS_KEY] = m_mass;
    j[SENSOR_KEY] = m_isSensor;
    j[GRAVITY_KEY] = m_gravityFactor;
}

void PhysicsBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");

    m_restitution = j.value(RESTITUTION_KEY, m_restitution);
    m_friction = j.value(FRICTION_KEY, m_friction);
    m_damping = j.value(DAMPING_KEY, m_damping);
    m_mass = j.value(MASS_KEY, m_mass);
    m_isSensor = j.value(SENSOR_KEY, m_isSensor);
    m_BodyType = j.value(TYPE_KEY, m_BodyType);
    m_gravityFactor = j.value(GRAVITY_KEY, m_gravityFactor);
}

bool PhysicsBodyComponent::isReadyToCreate(EntityContext &ctx) const {
    if (!m_PhysicsResource.isReady()) {
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
                     ? STATIC_COLOR
                     : DYNAMIC_COLOR;

    if (m_isSensor) {
        color = (m_BodyType == BODY_TYPE_STATIC)
                    ? STATIC_SENSOR_COLOR
                    : DYNAMIC_SENSOR_COLOR;
    }

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
            .setDebugColor(color)
            .setEntityReference(m_EntityId.id())
            .setTransform(*transformComponent)
            .setMass(m_mass)
            .setGravityFactor(m_gravityFactor)
            .setShape(*createdShape.Get().GetPtr())
            .setDamping(m_damping.x, m_damping.y)
            .setSensor(m_isSensor, m_excludeSensorFromActionHit);

    if (m_BodyType == BODY_TYPE_STATIC) {
        m_physicsBodyId = builder.createStatic()->GetID();
    } else {
        m_physicsBodyId = builder.createDynamic()->GetID();
    }

    m_PhysicsResource().getInterface().SetFriction(m_physicsBodyId, m_friction.x);
    m_PhysicsResource().getInterface().SetRestitution(m_physicsBodyId, m_restitution);
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
