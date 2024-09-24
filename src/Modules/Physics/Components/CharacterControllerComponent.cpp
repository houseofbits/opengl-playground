#include "CharacterControllerComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CharacterControllerSystem.h"
#include "../Helpers/PhysicsTypeCast.h"

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
                                                               m_rigidBody(nullptr),
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

    float mass = 1.0;
    auto *compoundShape = new btCompoundShape();
    auto *capsuleShape = new btCapsuleShape(m_radius, m_height - (m_radius * 2) - m_stepTolerance);
    btTransform localTransform;
    localTransform.setIdentity();
    localTransform.setOrigin(btVector3(0.0, (m_height * 0.5f), 0.0));
    compoundShape->addChildShape(localTransform, capsuleShape);

    btVector3 localInertia(0, 0, 0);
    auto *motionState = new btDefaultMotionState(PhysicsTypeCast::createBtTransformFromTransformComponent(transform));

    capsuleShape->calculateLocalInertia(mass, localInertia);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, compoundShape, localInertia);
    m_rigidBody = new btRigidBody(rbInfo);
    m_rigidBody->setAngularFactor(btVector3(0, 0, 0));
    m_rigidBody->setFriction(0.5f);

    auto *userData = new PhysicsUserData(m_EntityId.id());
    userData->m_contactReporting = true;
    m_rigidBody->setUserPointer(userData);

    m_rigidBody->setDamping(1, 1);

    m_PhysicsResource().m_dynamicsWorld->addRigidBody(m_rigidBody);
}

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void CharacterControllerComponent::update(TransformComponent &transform, bool isSimulationEnabled) {
    if (!isSimulationEnabled) {
        auto capsuleTransform = PhysicsTypeCast::createBtTransformFromTranslation(
                transform.getTranslation() + glm::vec3(0, m_stepTolerance, 0));

        m_rigidBody->setWorldTransform(capsuleTransform);
        m_rigidBody->getMotionState()->setWorldTransform(capsuleTransform);
        m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
        m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
        m_rigidBody->activate();
    } else {
        btTransform t;
        m_rigidBody->getMotionState()->getWorldTransform(t);
        glm::vec3 kneePoint = PhysicsTypeCast::bulletToGlm(t.getOrigin());

        transform.resetTransform();
        transform.setTranslation(kneePoint - glm::vec3(0, m_stepTolerance, 0));

        if (castRayForGroundReference(transform.getTranslation() + glm::vec3(0, m_stepTolerance * 2, 0))) {
            updateGroundSpring(kneePoint);
        }
    }
}

void CharacterControllerComponent::move(glm::vec3 direction) const {
    m_rigidBody->applyCentralForce(PhysicsTypeCast::glmToBullet(direction * 500.0f));
}

void CharacterControllerComponent::updateGroundSpring(const glm::vec3 &kneePoint) {
    m_rigidBody->activate();
    auto vel = m_rigidBody->getLinearVelocity();

    float velAlongSpring = vel.dot({0, 1, 0});

    glm::vec3 diff = kneePoint - m_groundRayCast.m_touchPoint;

    float positionDifference = m_stepTolerance - diff.y;
    m_isOnGround = std::abs(positionDifference) < 0.1;

    btScalar springForce = 2000.0f * positionDifference;
    btScalar dampingForce = -5.0f * velAlongSpring;
    btVector3 totalForce = btVector3(0, 1, 0) * (springForce + dampingForce);

    m_rigidBody->applyCentralForce(totalForce);
}

bool CharacterControllerComponent::castRayForGroundReference(const glm::vec3 &point) {
    return m_PhysicsResource().characterRayCast(point,
                                                glm::vec3(0, -1, 0),
                                                m_EntityId.id(),
                                                m_groundRayCast);
}
