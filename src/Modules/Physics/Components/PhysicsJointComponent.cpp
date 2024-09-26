#include "PhysicsJointComponent.h"

#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/JointsProcessingSystem.h"
#include <utility>
#include "../Helpers/PhysicsTypeCast.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>

//float normalizeAngle(float theta) {
//    while (theta > PxPi) {
//        theta -= 2.0f * PxPi;
//    }
//    while (theta < -PxPi) {
//        theta += 2.0f * PxPi;
//    }
//    return theta;
//}

PhysicsJointComponent::PhysicsJointComponent() : Component(),
                                                 m_PhysicsResource(),
                                                 m_targetEntityName(),
                                                 m_Joint(nullptr),
                                                 m_areLimitsEnabled(true),
                                                 m_areDriveEnabled(true),
                                                 m_angularLimits(0, 1.0),
                                                 m_axisA(0, 1, 0),
                                                 m_axisB(0, 1, 0),
                                                 m_localAttachmentA(0, 0, 0),
                                                 m_localAttachmentB(0, 0, 0),
                                                 m_moveState(UNKNOWN) {
}

void PhysicsJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
    j[ARE_LIMITS_ENABLED_KEY] = m_areLimitsEnabled;
    j[ARE_DRIVE_ENABLED_KEY] = m_areDriveEnabled;
    j[AXIS_A_KEY] = m_axisA;
    j[AXIS_B_KEY] = m_axisB;
    j[ATTACHMENT_A_KEY] = m_localAttachmentA;
    j[ATTACHMENT_B_KEY] = m_localAttachmentB;
    j[LIMITS_KEY] = m_angularLimits;
}

void PhysicsJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {

    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);
    m_areLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_areLimitsEnabled);
    m_areDriveEnabled = j.value(ARE_DRIVE_ENABLED_KEY, m_areDriveEnabled);
    m_axisA = j.value(AXIS_A_KEY, m_axisA);
    m_axisB = j.value(AXIS_B_KEY, m_axisB);
    m_localAttachmentA = j.value(ATTACHMENT_A_KEY, m_localAttachmentA);
    m_localAttachmentB = j.value(ATTACHMENT_B_KEY, m_localAttachmentB);
    m_angularLimits = j.value(LIMITS_KEY, m_angularLimits);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsJointComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<JointsProcessingSystem>(this);
}

bool PhysicsJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!bodyA.isCreated() || !bodyB.isCreated()) {
        return;
    }

    if (bodyA.m_EntityId == bodyB.m_EntityId) {
        Log::warn("PhysicsJointComponent: joint bodies cannot be the same");

        return;
    }

    if (bodyA.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC &&
        bodyB.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC) {
        Log::warn("PhysicsJointComponent: both bodies are STATIC, joint will have no effect");

        return;
    }

    JPH::HingeConstraintSettings settings;
    settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
    settings.mPoint1 = PhysicsTypeCast::glmToJPH(m_localAttachmentA);
    settings.mPoint2 = PhysicsTypeCast::glmToJPH(m_localAttachmentB);
    settings.mHingeAxis1 = PhysicsTypeCast::glmToJPH(m_axisA);
    settings.mHingeAxis2 = PhysicsTypeCast::glmToJPH(m_axisB);
    settings.mNormalAxis1 = JPH::Vec3::sAxisX();
    settings.mNormalAxis2 = JPH::Vec3::sAxisX();

//    if(m_areLimitsEnabled) {
//        settings.mLimitsMin = m_angularLimits.x;
//        settings.mLimitsMax = m_angularLimits.y;
//    }

    auto *groupFilter = new JPH::GroupFilterTable(2);
    groupFilter->DisableCollision(0, 1);
    bodyA.m_physicsBody->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 0));
    bodyB.m_physicsBody->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 1));

    m_Joint = (JPH::HingeConstraint *) settings.Create(*bodyA.m_physicsBody, *bodyB.m_physicsBody);

    m_PhysicsResource().getSystem().AddConstraint(m_Joint);

    bodyA.wakeUp();
    bodyB.wakeUp();

//
//    //Issue with bullet - should create joints before bodies or run simulation after bodies are created, before joint creation
//    m_PhysicsResource().simulate();
//
////    m_Joint = new btHingeConstraint(*bodyA.m_rigidBody, *bodyB.m_rigidBody,
////                                    PhysicsTypeCast::glmToBullet(m_localAttachmentA),
////                                    PhysicsTypeCast::glmToBullet(m_localAttachmentB),
////                                    PhysicsTypeCast::glmToBullet(m_axisA),
////                                    PhysicsTypeCast::glmToBullet(m_axisB),
////                                    true);
//
//    m_Joint = new btHingeConstraint(*bodyA.m_rigidBody,
//                                    PhysicsTypeCast::glmToBullet(m_localAttachmentA),
//                                    PhysicsTypeCast::glmToBullet(m_axisA));
//
////    m_Joint->setParam(BT_CONSTRAINT_ERP, 0.8, -1);  // ERP value to correct error quickly
////    m_Joint->setParam(BT_CONSTRAINT_CFM, 0.01, -1); // Low CFM value for stiffness
//
//    // Enable hinge motor with high max impulse to act as damping
////    m_Joint->enableAngularMotor(true, 0, 100.0f);  // Zero target velocity, high max impulse to act as damping
//
////    if (m_areLimitsEnabled) {
//    m_Joint->setLimit(m_angularLimits.x, m_angularLimits.y, 0, 10, 0);
////    }
//
//    if (m_areDriveEnabled) {
//        m_Joint->enableAngularMotor(true, 1.0, 10.0);
//    }
//
//    m_PhysicsResource().m_dynamicsWorld->addConstraint(m_Joint, true);
}

void PhysicsJointComponent::attach(std::string entityName) {
    release();
    m_targetEntityName = std::move(entityName);
}

void PhysicsJointComponent::release() {
//    if (m_Joint != nullptr) {
//        m_PhysicsResource().m_dynamicsWorld->removeConstraint(m_Joint);
//        delete m_Joint;
//    }
}

void PhysicsJointComponent::activate() {
    if (!m_areDriveEnabled) {
        return;
    }

//    if (m_moveState == CLOSED) {
//        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
//        m_pxJoint->setDriveVelocity(10.0f);
//        m_moveState = OPENING;
//        std::cout << "OPEN" << std::endl;
//    }
//    if (m_moveState == OPEN) {
//        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
//        m_pxJoint->setDriveVelocity(-10.0f);
//        m_moveState = CLOSING;
//        std::cout << "CLOSE" << std::endl;
//    }
}

void PhysicsJointComponent::update() {
    if (!m_areDriveEnabled) {
        return;
    }

//    float angle = normalizeAngle(m_pxJoint->getAngle());
//    auto limit = m_pxJoint->getLimit();
//
////    std::cout << (angle) << "|" << normalizeAngle(angle) << " " << (limit.lower) << ", " << (limit.upper) << std::endl;
//    //        std::cout << glm::degrees(angle) << " " << glm::degrees(limit.lower) << ", " << glm::degrees(limit.upper) << std::endl;
//
//    if (m_moveState == UNKNOWN) {
//        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
//        m_pxJoint->setDriveVelocity(10.0f);
//        m_moveState = OPENING;
//
//        std::cout << "INITIAL CLOSE" << std::endl;
//    }
//
//    if (m_moveState == CLOSING && angle <= (limit.lower + 0.01)) {
//        m_moveState = CLOSED;
//        m_pxJoint->setDriveVelocity(0);
//        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, false);
//
//        std::cout << "IS CLOSED" << std::endl;
//    }
//    if (m_moveState == OPENING && angle >= (limit.upper - 0.01)) {
//        m_moveState = OPEN;
//        m_pxJoint->setDriveVelocity(0);
//        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, false);
//
//        std::cout << "IS OPEN" << std::endl;
//    }
}

bool PhysicsJointComponent::isCreated() const {
    return m_Joint != nullptr;
}
