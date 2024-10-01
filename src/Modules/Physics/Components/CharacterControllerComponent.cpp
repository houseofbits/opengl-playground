#include "CharacterControllerComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CharacterControllerSystem.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/Collision/CastResult.h"

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
                                                               m_physicsBody(nullptr),
                                                               m_haveGroundDetected(false),
                                                               m_minDistanceToGround(1000),
                                                               m_stepTolerance(0.2),
                                                               m_isOnGround(false),
                                                               m_PhysicsResource(),
                                                               m_groundSpringForce(100000.f),
                                                               m_groundSpringDamping(2000.f),
                                                               m_freeFallForce(50000.f) {
}

void CharacterControllerComponent::serialize(nlohmann::json &j) {
    j["height"] = m_height;
    j["radius"] = m_radius;
}

void CharacterControllerComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_height = j.value("height", m_height);
    m_radius = j.value("radius", m_radius);

    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<CharacterControllerSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void CharacterControllerComponent::create(TransformComponent &transform) {

    float cylinderHalfHeight = (m_height - m_stepTolerance - (m_radius * 2.f)) * 0.5f;
    float cylinderOffsetY = m_height - (cylinderHalfHeight + m_radius);

    auto mStandingShape = JPH::RotatedTranslatedShapeSettings(
            JPH::Vec3(0, cylinderOffsetY, 0),
            JPH::Quat::sIdentity(),
            new JPH::CapsuleShape(cylinderHalfHeight, m_radius))
            .Create()
            .Get();

    auto characterSettings = JPH::BodyCreationSettings(
            mStandingShape,
            PhysicsTypeCast::glmToJPH(transform.getTranslation()),
            PhysicsTypeCast::glmToJPH(transform.getRotation()),
            JPH::EMotionType::Dynamic,
            Layers::MOVING);

    characterSettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
    characterSettings.mMassPropertiesOverride.mMass = 80;
    characterSettings.mAllowedDOFs =
            JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY | JPH::EAllowedDOFs::TranslationZ;
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

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void CharacterControllerComponent::update(TransformComponent &transform, bool isSimulationEnabled) {
    if (isSimulationEnabled) {
        auto t = m_physicsBody->GetWorldTransform();
        PhysicsTypeCast::applyJPHMat44ToTransformComponent(transform, t);

        castRayForGroundReference(transform.getTranslation());
        updateGroundSpring(transform.getTranslation());

        transform.setTranslation(glm::vec3(0, -m_stepTolerance, 0));
    } else {
        m_PhysicsResource().getInterface().SetPositionAndRotation(m_physicsBody->GetID(),
                                                                  PhysicsTypeCast::glmToJPH(transform.getTranslation() +
                                                                                            glm::vec3(0,
                                                                                                      m_stepTolerance,
                                                                                                      0)),
                                                                  PhysicsTypeCast::glmToJPH(transform.getRotation()),
                                                                  JPH::EActivation::DontActivate);
    }
}

void CharacterControllerComponent::move(glm::vec3 direction) {
    float speed = 10.0f;

    JPH::Vec3 movementDirection = PhysicsTypeCast::glmToJPH(direction);

    JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();
    JPH::Vec3 desiredVelocity = speed * movementDirection;
    if (!desiredVelocity.IsNearZero() || currentVelocity.GetY() < 0.0f) {
        desiredVelocity.SetY(currentVelocity.GetY());
    }

    JPH::Vec3 newVelocity = 0.75f * currentVelocity + 0.25f * desiredVelocity;

    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());
    m_physicsBody->SetLinearVelocity(newVelocity);
}

void CharacterControllerComponent::updateGroundSpring(const glm::vec3 &kneePosition) {
    m_isOnGround = false;

    if (!m_haveGroundDetected) {
        m_physicsBody->AddForce(JPH::Vec3(0, -m_freeFallForce , 0) );
        return;
    }

    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());
    JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();
    float velAlongSpring = currentVelocity.Dot({0, 1, 0});
    float positionDifference = m_stepTolerance - m_minDistanceToGround;
    float springForce = m_groundSpringForce * positionDifference;
    float dampingForce = -m_groundSpringDamping * velAlongSpring;
    JPH::Vec3 totalForce = JPH::Vec3(0, 1, 0) * (springForce + dampingForce);

    m_physicsBody->AddForce(totalForce);

    m_isOnGround = std::abs(positionDifference) < 0.1;
}

bool CharacterControllerComponent::isCreated() const {
    return m_physicsBody != nullptr;
}

void CharacterControllerComponent::castRayForGroundReference(const glm::vec3 &point) {
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
    glm::vec3 fw = glm::vec3(m_lookingDirection.x, 0, m_lookingDirection.z) * radiusFactor;
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

    glm::vec3 right = glm::normalize(glm::cross(fw, glm::vec3(0,1,0))) * radiusFactor;
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

bool CharacterControllerComponent::rayCast(glm::vec3 position, glm::vec3 direction, PhysicsRayCastResult &result) {
    JPH::RayCastResult hit;
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(position), PhysicsTypeCast::glmToJPH(direction)};
    JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, {}, bodyFilter)) {
        auto *userData = reinterpret_cast<PhysicsUserData *>(m_PhysicsResource().getInterface().GetUserData(
                hit.mBodyID));
        result.m_entityId = userData->m_entityId;
        result.m_touchPoint = PhysicsTypeCast::JPHToGlm(ray.GetPointOnRay(hit.mFraction));
        result.m_distance = ray.mDirection.Length() * hit.mFraction;

        return true;
    }

    return false;
}

glm::vec3 CharacterControllerComponent::getVelocity() const {
    return PhysicsTypeCast::JPHToGlm(m_physicsBody->GetLinearVelocity());
}

void CharacterControllerComponent::setLookingDirection(glm::vec3 &direction) {
    m_lookingDirection = direction;
}
