#include "PhysicsCharacterComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/SensorLayerFilter.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>

PhysicsCharacterComponent::PhysicsCharacterComponent() : PhysicsComponent(),
                                                         m_height(1.75),
                                                         m_radius(0.25),
                                                         m_PhysicsResource(),
                                                         m_isOnGround(false),
                                                         m_stepTolerance(0.2),
                                                         m_physicsBody(nullptr),
                                                         m_haveGroundDetected(false),
                                                         m_minDistanceToGround(1000),
                                                         m_moveDirection(0, 0, 1),
                                                         m_groundSpringForce(100000.f),
                                                         m_groundSpringDamping(2000.f),
                                                         m_freeFallForce(50000.f),
                                                         m_doMove(false),
                                                         m_movementDirection(0) {
}

void PhysicsCharacterComponent::serialize(nlohmann::json &j) {
    j["height"] = m_height;
    j["radius"] = m_radius;
}

void PhysicsCharacterComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_height = j.value("height", m_height);
    m_radius = j.value("radius", m_radius);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsCharacterComponent::create(TransformComponent &transform) {
    const float cylinderHalfHeight = (m_height - m_stepTolerance - (m_radius * 2.f)) * 0.5f;
    Log::write("st:", m_stepTolerance, ", chh:", cylinderHalfHeight, ", h:", m_height, ", r: ", m_radius);
    const float cylinderOffsetY = m_height - (cylinderHalfHeight + m_radius);

    const auto material = new JPH::PhysicsMaterialSimple("Material2", JPH::Color(0, 0, 255));
    const auto mStandingShape = JPH::RotatedTranslatedShapeSettings(
                JPH::Vec3(0, cylinderOffsetY, 0),
                JPH::Quat::sIdentity(),
                new JPH::CapsuleShape(cylinderHalfHeight, m_radius, material))
            .Create()
            .Get();

    auto characterSettings = JPH::BodyCreationSettings(
        mStandingShape,
        PhysicsTypeCast::glmToJPH(transform.getWorldPosition()),
        PhysicsTypeCast::glmToJPH(transform.getRotation()),
        JPH::EMotionType::Dynamic,
        Layers::MOVING);

    characterSettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
    characterSettings.mMassPropertiesOverride.mMass = 80;
    characterSettings.mAllowedDOFs =
            JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY | JPH::EAllowedDOFs::TranslationZ;
    // | JPH::EAllowedDOFs::RotationY;
    characterSettings.mLinearDamping = 10;
    characterSettings.mAngularDamping = 10;
    //    characterSettings.mNumPositionStepsOverride = 255;
    //    characterSettings.mNumVelocityStepsOverride = 255;

    m_physicsBody = m_PhysicsResource().getInterface().CreateBody(characterSettings);

    m_physicsBody->SetFriction(0.5);
    m_physicsBody->SetRestitution(0);

    auto *userData = new PhysicsUserData(m_EntityId.id());
    m_physicsBody->SetUserData(reinterpret_cast<unsigned long>(userData));

    m_PhysicsResource().getInterface().AddBody(m_physicsBody->GetID(), JPH::EActivation::Activate);
}

bool PhysicsCharacterComponent::isReadyToCreate(EntityContext &ctx) const {
    if (!m_PhysicsResource.isReady()) {
        return false;
    }

    if (!ctx.getEntityComponent<TransformComponent>(m_EntityId.id())) {
        return false;
    }

    return true;
}

void PhysicsCharacterComponent::createPhysics(EntityContext &ctx) {
    if (const auto transformComponent = ctx.getEntityComponent<TransformComponent>(m_EntityId.id())) {
        create(*transformComponent);

        return;
    }

    Log::warn("PhysicsCharacterComponent::initialize transform component not found");
}

void PhysicsCharacterComponent::update(TransformComponent &transform, bool isSimulationEnabled) {
    if (isSimulationEnabled) {
        auto t = m_physicsBody->GetWorldTransform();

        //Rotation from look-at direction
        constexpr glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
        const glm::vec3 zAxis = glm::normalize(glm::vec3(m_moveDirection.x, 0, m_moveDirection.z));
        const glm::vec3 xAxis = glm::cross(yAxis, zAxis);
        const glm::mat4 rotationMatrix(
            glm::vec4(xAxis, 1),
            glm::vec4(yAxis, 1),
            glm::vec4(zAxis, 1),
            glm::vec4(0, 0, 0, 1));

        const auto capsulePosition = PhysicsTypeCast::JPHToGlm(t.GetTranslation());

        auto transformMatrix = glm::mat4(1.0);
        transformMatrix = glm::translate(transformMatrix, capsulePosition - glm::vec3(0, m_stepTolerance, 0));
        transformMatrix *= rotationMatrix;
        transform.setWorldTransform(transformMatrix);

        //
        // // glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat_cast(rotationMatrix));
        //
        // transform.resetTransform();
        // transform.setTranslation(capsulePosition - glm::vec3(0, m_stepTolerance, 0));
        // transform.setRotation(glm::quat_cast(rotationMatrix));

        castRayForGroundReference(capsulePosition);
        updateGroundSpring(capsulePosition);

        updateMove();

        // auto t = m_physicsBody->GetWorldTransform();
        // PhysicsTypeCast::applyJPHMat44ToTransformComponent(transform, t);
        //
        // castRayForGroundReference(transform.getWorldPosition());
        // updateGroundSpring(transform.getWorldPosition());
        //
        // transform.setTranslation(glm::vec3(0, -m_stepTolerance, 0));
        //
        // updateMove();
    } else {
        // m_PhysicsResource().getInterface().SetPositionAndRotation(m_physicsBody->GetID(),
        //                                                           PhysicsTypeCast::glmToJPH(
        //                                                               transform.getWorldPosition() +
        //                                                               glm::vec3(0,
        //                                                                   m_stepTolerance,
        //                                                                   0)),
        //                                                           PhysicsTypeCast::glmToJPH(transform.getRotation()),
        //                                                           JPH::EActivation::DontActivate);
    }
}

void PhysicsCharacterComponent::move(glm::vec3 direction) {
    m_movementDirection = glm::normalize(m_movementDirection + direction);
    m_doMove = true;
}

void PhysicsCharacterComponent::updateGroundSpring(const glm::vec3 &kneePosition) {
    m_isOnGround = false;

    if (!m_haveGroundDetected) {
        m_physicsBody->AddForce(JPH::Vec3(0, -m_freeFallForce, 0));
        return;
    }

    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());
    const JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();
    const float velAlongSpring = currentVelocity.Dot({0, 1, 0});
    const float positionDifference = m_stepTolerance - m_minDistanceToGround;
    const float springForce = m_groundSpringForce * positionDifference;
    const float dampingForce = -m_groundSpringDamping * velAlongSpring;
    const JPH::Vec3 totalForce = JPH::Vec3(0, 1, 0) * (springForce + dampingForce);

    m_physicsBody->AddForce(totalForce);

    m_isOnGround = std::abs(positionDifference) < 0.1;
}

bool PhysicsCharacterComponent::isCreated() const {
    return m_physicsBody != nullptr;
}

void PhysicsCharacterComponent::castRayForGroundReference(const glm::vec3 &point) {
    m_isOnGround = false;
    m_haveGroundDetected = false;
    float prevDist = 1;
    float rayCastStart = m_stepTolerance;
    float radiusFactor = m_radius * .5f;
    glm::vec3 direction(0, -prevDist, 0);
    glm::vec3 p = point;
    PhysicsRayCastResult rayCastResult;
    if (rayCast(p + glm::vec3(0, rayCastStart, 0), direction, rayCastResult)) {
        prevDist = rayCastResult.m_distance;
        m_haveGroundDetected = true;
        m_minDistanceToGround = glm::length(point - rayCastResult.m_touchPoint);
    }

    //Forward
    glm::vec3 fw = glm::vec3(m_moveDirection.x, 0, m_moveDirection.z) * radiusFactor;
    p = point + fw;
    if (rayCast(p + glm::vec3(0, rayCastStart, 0), direction, rayCastResult)
        && rayCastResult.m_distance < prevDist) {
        prevDist = rayCastResult.m_distance;
        m_haveGroundDetected = true;
        m_minDistanceToGround = glm::length(p - rayCastResult.m_touchPoint);
    }

    //Backward
    p = point - fw;
    if (rayCast(p + glm::vec3(0, rayCastStart, 0), direction, rayCastResult)
        && rayCastResult.m_distance < prevDist) {
        prevDist = rayCastResult.m_distance;
        m_haveGroundDetected = true;
        m_minDistanceToGround = glm::length(p - rayCastResult.m_touchPoint);
    }

    glm::vec3 right = glm::normalize(glm::cross(fw, glm::vec3(0, 1, 0))) * radiusFactor;
    //Right
    p = point + right;
    if (rayCast(p + glm::vec3(0, rayCastStart, 0), direction, rayCastResult)
        && rayCastResult.m_distance < prevDist) {
        prevDist = rayCastResult.m_distance;
        m_haveGroundDetected = true;
        m_minDistanceToGround = glm::length(p - rayCastResult.m_touchPoint);
    }

    //Left
    p = point - right;
    if (rayCast(p + glm::vec3(0, rayCastStart, 0), direction, rayCastResult)
        && rayCastResult.m_distance < prevDist) {
        prevDist = rayCastResult.m_distance;
        m_haveGroundDetected = true;
        m_minDistanceToGround = glm::length(p - rayCastResult.m_touchPoint);
    }
}

bool PhysicsCharacterComponent::rayCast(glm::vec3 position, glm::vec3 direction, PhysicsRayCastResult &result) {
    JPH::RayCastResult hit;
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(position), PhysicsTypeCast::glmToJPH(direction)};
    JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    SensorLayerFilter filter{};

    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, filter, bodyFilter)) {
        auto *userData = m_PhysicsResource().getBodyUserData(hit.mBodyID);
        result.m_entityId = userData->m_entityId;
        result.m_touchPoint = PhysicsTypeCast::JPHToGlm(ray.GetPointOnRay(hit.mFraction));
        result.m_distance = ray.mDirection.Length() * hit.mFraction;

        return true;
    }

    return false;
}

glm::vec3 PhysicsCharacterComponent::getVelocity() const {
    return PhysicsTypeCast::JPHToGlm(m_physicsBody->GetLinearVelocity());
}

void PhysicsCharacterComponent::setMoveDirection(const glm::vec3 &direction) {
    m_moveDirection = direction;
}

void PhysicsCharacterComponent::updateMove() {
    if (!m_doMove) {
        return;
    }

    constexpr float speed = 10.0f;

    JPH::Vec3 movementDirection = PhysicsTypeCast::glmToJPH(m_movementDirection);

    JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();
    JPH::Vec3 desiredVelocity = speed * movementDirection;
    if (!desiredVelocity.IsNearZero() || currentVelocity.GetY() < 0.0f) {
        desiredVelocity.SetY(currentVelocity.GetY());
    }

    const JPH::Vec3 newVelocity = 0.75f * currentVelocity + 0.25f * desiredVelocity;

    m_physicsBody->SetLinearVelocity(newVelocity);
    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());

    m_doMove = false;
    m_movementDirection = glm::vec3(0.0f);
}
