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
                                                               m_groundRayCast(),
                                                               m_stepTolerance(0.3),
                                                               m_isOnGround(false),
                                                               m_PhysicsResource() {
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

    auto mStandingShape = JPH::RotatedTranslatedShapeSettings(
            JPH::Vec3(0, 0.5f * m_height + m_radius, 0),
            JPH::Quat::sIdentity(),
            new JPH::CapsuleShape(0.5f * m_height, m_radius))
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

    m_PhysicsResource().getInterface().AddBody(m_physicsBody->GetID(), JPH::EActivation::Activate);

    m_physicsBody->SetFriction(0.5);
    m_physicsBody->SetRestitution(0);
}

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void CharacterControllerComponent::update(TransformComponent &transform, bool isSimulationEnabled) {
    if (isSimulationEnabled) {
        auto t = m_physicsBody->GetWorldTransform();
        PhysicsTypeCast::applyJPHMat44ToTransformComponent(transform, t);

        castRayForGroundReference(transform.getTranslation());
        updateGroundSpring(transform.getTranslation());// + glm::vec3(0,m_stepTolerance,0));
    } else {
        m_PhysicsResource().getInterface().SetPositionAndRotation(m_physicsBody->GetID(),
                                                                  PhysicsTypeCast::glmToJPH(transform.getTranslation()),
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
    m_PhysicsResource().getInterface().ActivateBody(m_physicsBody->GetID());
    JPH::Vec3 currentVelocity = m_physicsBody->GetLinearVelocity();
    float velAlongSpring = currentVelocity.Dot({0, 1, 0});

    glm::vec3 diff = kneePosition - m_groundRayCast.m_touchPoint;
    float positionDifference = m_stepTolerance - diff.y;
    float springForce = 10000.0f * positionDifference;
    float dampingForce = -50.0f * velAlongSpring;
    JPH::Vec3 totalForce = JPH::Vec3(0, 1, 0) * (springForce + dampingForce);

    m_physicsBody->AddForce(totalForce);
}

bool CharacterControllerComponent::isCreated() const {
    return m_physicsBody != nullptr;
}

//Refactor into public method
void CharacterControllerComponent::castRayForGroundReference(const glm::vec3 &point) {
    m_isOnGround = false;

    JPH::RayCastResult hit;
    JPH::Vec3 rayDirection(0, -10, 0);
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(point + glm::vec3(0, m_stepTolerance * 2, 0)), rayDirection};
    JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, {}, bodyFilter)) {
        m_groundRayCast.m_touchPoint = PhysicsTypeCast::JPHToGlm(ray.GetPointOnRay(hit.mFraction));
        m_groundRayCast.m_distance = glm::length(point - m_groundRayCast.m_touchPoint);

        if (m_groundRayCast.m_distance < 0.1) {
            m_isOnGround = true;
        }
    }
}

bool CharacterControllerComponent::rayCast(glm::vec3 position, glm::vec3 direction, PhysicsRayCastResult &result) {
    JPH::RayCastResult hit;
    JPH::RRayCast ray{PhysicsTypeCast::glmToJPH(position), PhysicsTypeCast::glmToJPH(direction)};
    JPH::IgnoreSingleBodyFilter bodyFilter(m_physicsBody->GetID());
    if (m_PhysicsResource().getSystem().GetNarrowPhaseQuery().CastRay(ray, hit, {}, {}, bodyFilter)) {
        result.m_touchPoint = PhysicsTypeCast::JPHToGlm(ray.GetPointOnRay(hit.mFraction));
        result.m_distance = ray.mDirection.Length() * hit.mFraction;
        //TODO Get user data

        return true;
    }

    return false;
}

glm::vec3 CharacterControllerComponent::getVelocity() const {
    return PhysicsTypeCast::JPHToGlm(m_physicsBody->GetLinearVelocity());
}
