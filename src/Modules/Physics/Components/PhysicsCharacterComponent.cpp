#include "PhysicsCharacterComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/SensorLayerFilter.h"
#include "../Helpers/PhysicsShapeUserData.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>

#include "../../../Core/Helper/Math.h"

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
    //Log::write("st:", m_stepTolerance, ", chh:", cylinderHalfHeight, ", h:", m_height, ", r: ", m_radius);
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
            JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY | JPH::EAllowedDOFs::TranslationZ
    | JPH::EAllowedDOFs::RotationY;

    // characterSettings.mLinearDamping = 10; //Damping slows down the player (makes him lag behind) when riding on a platform, for example
    characterSettings.mAngularDamping = 30;

    // characterSettings.mNumPositionStepsOverride = 255;
    // characterSettings.mNumVelocityStepsOverride = 255;

    m_physicsBody = m_PhysicsResource().getInterface().CreateBody(characterSettings);

    m_physicsBody->SetFriction(0);
    // m_physicsBody->SetRestitution(0);

    auto *userData = new PhysicsBodyUserData(m_EntityId.id());
    m_physicsBody->SetUserData(reinterpret_cast<unsigned long long>(userData));

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
        const auto t = m_physicsBody->GetWorldTransform();
        const auto capsulePosition = PhysicsTypeCast::JPHToGlm(t.GetTranslation());

        auto transformMatrix = glm::mat4(1.0);
        transformMatrix = glm::translate(transformMatrix, capsulePosition - glm::vec3(0, m_stepTolerance, 0));

        auto rot = t.GetRotation().GetQuaternion();
        glm::quat glmQuat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());

        transformMatrix *= glm::toMat4(glmQuat); //rotationMatrix;
        transform.setWorldTransform(transformMatrix);

        castRayForGroundReference(capsulePosition);
        updateGroundSpring(capsulePosition);

        updateMove();

        // Log::write(Math::getTranslation(transformMatrix), " / ", capsulePosition);
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
    m_isGroundMoving = false;
    float prevDist = 1;
    const float rayCastStart = m_stepTolerance;
    const float radiusFactor = m_radius * .5f;
    glm::vec3 direction(0, -prevDist, 0);
    glm::vec3 p = point;
    PhysicsRayCastResult rayCastResult;
    //Center
    auto result = rayCastFeet(p + glm::vec3(0, rayCastStart, 0), direction, true);
    if (result.has_value() && result.value() < prevDist) {
        prevDist = result.value();
        m_haveGroundDetected = true;
        m_minDistanceToGround = result.value() - rayCastStart;
    }

    //Forward
    glm::vec3 fw = glm::vec3(m_moveDirection.x, 0, m_moveDirection.z) * radiusFactor;
    p = point + fw;
    result = rayCastFeet(p + glm::vec3(0, rayCastStart, 0), direction);
    if (result.has_value() && result.value() < prevDist) {
        prevDist = result.value();
        m_haveGroundDetected = true;
        m_minDistanceToGround = result.value() - rayCastStart;
    }

    //Backward
    p = point - fw;
    result = rayCastFeet(p + glm::vec3(0, rayCastStart, 0), direction);
    if (result.has_value() && result.value() < prevDist) {
        prevDist = result.value();
        m_haveGroundDetected = true;
        m_minDistanceToGround = result.value() - rayCastStart;
    }

    glm::vec3 right = glm::normalize(glm::cross(fw, glm::vec3(0, 1, 0))) * radiusFactor;
    //Right
    p = point + right;
    result = rayCastFeet(p + glm::vec3(0, rayCastStart, 0), direction);
    if (result.has_value() && result.value() < prevDist) {
        prevDist = result.value();
        m_haveGroundDetected = true;
        m_minDistanceToGround = result.value() - rayCastStart;
    }

    //Left
    p = point - right;
    result = rayCastFeet(p + glm::vec3(0, rayCastStart, 0), direction);
    if (result.has_value() && result.value() < prevDist) {
        prevDist = result.value();
        m_haveGroundDetected = true;
        m_minDistanceToGround = result.value() - rayCastStart;
    }
}

bool PhysicsCharacterComponent::rayCast(const glm::vec3 position, const glm::vec3 direction,
                                        PhysicsRayCastResult &result, const bool excludeAllSensors) {
    JPH::RayCastResult hit;
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(position), PhysicsTypeCast::glmToJPH(direction)};
    const JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    SensorLayerFilter filter{excludeAllSensors};

    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, filter, bodyFilter)) {
        if (const JPH::BodyLockRead lock(m_PhysicsResource().getLockInterface(), hit.mBodyID); lock.Succeeded()) {
            const auto rawUserData = lock.GetBody().GetShape()->GetSubShapeUserData(hit.mSubShapeID2);
            const auto userData = reinterpret_cast<PhysicsShapeUserData *>(rawUserData);
            result.m_shapeComponentId = userData->m_physicsShapeComponentId;
            result.m_shapeComponentName = userData->m_physicsShapeComponentName;
            //
            // if (lock.GetBody().GetMotionType() == JPH::EMotionType::Dynamic) {
            // }
        }

        const auto *userData = m_PhysicsResource().getBodyUserData(hit.mBodyID);
        result.m_entityId = userData->m_entityId;
        result.m_touchPoint = PhysicsTypeCast::JPHToGlm(ray.GetPointOnRay(hit.mFraction));
        result.m_distance = ray.mDirection.Length() * hit.mFraction;

        return true;
    }

    return false;
}

std::optional<float> PhysicsCharacterComponent::rayCastFeet(const glm::vec3 position, const glm::vec3 direction,
                                                            bool doCheckGroundMovement) {
    JPH::RayCastResult hit;
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(position), PhysicsTypeCast::glmToJPH(direction)};
    const JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    SensorLayerFilter filter{true};

    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, filter, bodyFilter)) {
        if (doCheckGroundMovement) {
            if (const JPH::BodyLockRead lock(m_PhysicsResource().getLockInterface(), hit.mBodyID); lock.Succeeded()) {
                auto &body = lock.GetBody();
                if (body.GetMotionType() == JPH::EMotionType::Dynamic) {
                    const auto point = ray.GetPointOnRay(hit.mFraction);
                    const auto velocityAtPoint = body.GetPointVelocity(point);
                    if (velocityAtPoint.Length() > 0.1) {
                        m_isGroundMoving = true;
                        m_groundMovementVelocity = velocityAtPoint;
                    }
                }
            }
        }

        return std::optional(ray.mDirection.Length() * hit.mFraction);
    }
    return std::nullopt;
}

glm::vec3 PhysicsCharacterComponent::getVelocity() const {
    return PhysicsTypeCast::JPHToGlm(m_physicsBody->GetLinearVelocity());
}

void PhysicsCharacterComponent::setMoveDirection(const glm::vec3 &direction) {
    m_moveDirection = direction;
}

void PhysicsCharacterComponent::updateMove() {
    if (!m_isOnGround) {
        return;
    }

    constexpr float dampingFactor = 0.9f;
    JPH::Vec3 inputVelocity = {0.0, 0.0, 0.0};

    if (m_doMove) {
        constexpr float speed = 10.0f;
        inputVelocity = PhysicsTypeCast::glmToJPH(m_movementDirection) * speed;
    }

    JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();

    if (m_isGroundMoving) {
        currentVelocity -= m_groundMovementVelocity;
    }

    JPH::Vec3 newVelocity = dampingFactor * currentVelocity + (1.0 - dampingFactor) * inputVelocity;

    if (m_isGroundMoving) {
        newVelocity += m_groundMovementVelocity;
    }

    m_physicsBody->SetLinearVelocityClamped(newVelocity);
    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());

    m_doMove = false;
    m_movementDirection = glm::vec3(0.0f);

    auto angularVel = m_physicsBody->GetAngularVelocity();
    constexpr float angularDampingFactor = 0.0001f;
    constexpr float rotationSpeed = 0.3f;
    const float yVel = m_rotationDirection * rotationSpeed;

    // angularVel.SetY(angularDampingFactor * angularVel.GetY() + (1.0 - angularDampingFactor) * yVel);
    angularVel.SetY(yVel);
    m_physicsBody->SetAngularVelocityClamped(angularVel);

    m_rotationDirection = 0;

}
