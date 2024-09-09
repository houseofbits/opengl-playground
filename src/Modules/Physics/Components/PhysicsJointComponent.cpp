#include "PhysicsJointComponent.h"

#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/JointsProcessingSystem.h"
#include <utility>

using namespace physx;

float normalizeAngle(float theta) {
    while (theta > PxPi) {
        theta -= 2.0f * PxPi;
    }
    while (theta < -PxPi) {
        theta += 2.0f * PxPi;
    }
    return theta;
}

PhysicsJointComponent::PhysicsJointComponent() : Component(),
                                                 m_PhysicsResource(),
                                                 m_targetEntityName(),
                                                 m_pxJoint(nullptr),
                                                 m_areLimitsEnabled(true),
                                                 m_areDriveEnabled(true),
                                                 m_angularLimits(0, 1.0),
                                                 m_axis(0, 1, 0),
                                                 m_localAttachmentA(0, 0, 0),
                                                 m_localAttachmentB(0, 0, 0),
                                                 m_moveState(UNKNOWN) {
}

void PhysicsJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
    j[ARE_LIMITS_ENABLED_KEY] = m_areLimitsEnabled;
    j[ARE_DRIVE_ENABLED_KEY] = m_areDriveEnabled;
    j[AXIS_KEY] = m_axis;
    j[ATTACHMENT_A_KEY] = m_localAttachmentA;
    j[ATTACHMENT_B_KEY] = m_localAttachmentB;
    j[LIMITS_KEY] = m_angularLimits;
}

void PhysicsJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {

    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);
    m_areLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_areLimitsEnabled);
    m_areDriveEnabled = j.value(ARE_DRIVE_ENABLED_KEY, m_areDriveEnabled);
    m_axis = j.value(AXIS_KEY, m_axis);
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
    if (bodyA.m_pxRigidActor == nullptr || bodyB.m_pxRigidActor == nullptr) {
        return;
    }

    if (bodyA.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC && bodyB.m_BodyType == PhysicsBodyComponent::BODY_TYPE_STATIC) {
        Log::info("PhysicsJointComponent: both bodies are STATIC, joint will have no effect");
    }

    physx::PxQuat axis = calculateAxisRotation();//PxQuat(physx::PxPi / 2, PxVec3(0.0f, 0.0f, 1.0f));//
    physx::PxTransform localFrame1 = physx::PxTransform(Types::GLMtoPxVec3(m_localAttachmentA), axis);
    physx::PxTransform localFrame2 = physx::PxTransform(Types::GLMtoPxVec3(m_localAttachmentB), axis);

    m_pxJoint = physx::PxRevoluteJointCreate(*m_PhysicsResource().m_pxPhysics,
                                             bodyA.m_pxRigidActor, localFrame1,
                                             bodyB.m_pxRigidActor, localFrame2);

//    if (m_areLimitsEnabled) {
        physx::PxJointAngularLimitPair limits(m_angularLimits.x, m_angularLimits.y);
        //        physx::PxJointAngularLimitPair limits(-physx::PxPi / 2, physx::PxPi / 2);

        limits.restitution = 0;
        limits.bounceThreshold = 0;
//        limits.stiffness = 50;
//        limits.damping = 5;

        m_pxJoint->setDriveForceLimit(1000);
        m_pxJoint->setLimit(limits);
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);

//    }
}

void PhysicsJointComponent::attach(std::string entityName) {
    release();
    m_targetEntityName = std::move(entityName);
}

void PhysicsJointComponent::release() {
    if (m_pxJoint != nullptr) {
        m_pxJoint->release();
    }
    m_pxJoint = nullptr;
}

physx::PxQuat PhysicsJointComponent::calculateAxisRotation() const {
    PxVec3 aNorm(1, 0, 0);
    PxVec3 bNorm = Types::GLMtoPxVec3(m_axis);

    PxVec3 axis = aNorm.cross(bNorm);

    if (axis.magnitude() < 1e-6) {
        float dot = aNorm.dot(bNorm);
        if (dot > 0.9999f) {
            return {PxIdentity};
        } else {
            PxVec3 orthoAxis = PxVec3(1, 0, 0).cross(aNorm);
            if (orthoAxis.magnitude() < 1e-6) {
                orthoAxis = PxVec3(0, 1, 0).cross(aNorm);
            }
            orthoAxis.normalize();
            return {PxPi, orthoAxis};
        }
    }
    axis.normalize();

    float dot = aNorm.dot(bNorm);
    float angle = PxAcos(dot);

    PxQuat rotationQuat(angle, axis);

    return rotationQuat;
}

void PhysicsJointComponent::activate() {
    if (!m_areDriveEnabled) {
        return;
    }

    if (m_moveState == CLOSED) {
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
        m_pxJoint->setDriveVelocity(10.0f);
        m_moveState = OPENING;
        std::cout << "OPEN" << std::endl;
    }
    if (m_moveState == OPEN) {
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
        m_pxJoint->setDriveVelocity(-10.0f);
        m_moveState = CLOSING;
        std::cout << "CLOSE" << std::endl;
    }
}

void PhysicsJointComponent::update() {
    if (!m_areDriveEnabled) {
        return;
    }

    float angle = normalizeAngle(m_pxJoint->getAngle());
    auto limit = m_pxJoint->getLimit();

//    std::cout << (angle) << "|" << normalizeAngle(angle) << " " << (limit.lower) << ", " << (limit.upper) << std::endl;
    //        std::cout << glm::degrees(angle) << " " << glm::degrees(limit.lower) << ", " << glm::degrees(limit.upper) << std::endl;

    if (m_moveState == UNKNOWN) {
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, true);
        m_pxJoint->setDriveVelocity(10.0f);
        m_moveState = OPENING;

        std::cout << "INITIAL CLOSE" << std::endl;
    }

    if (m_moveState == CLOSING && angle <= (limit.lower + 0.01)) {
        m_moveState = CLOSED;
        m_pxJoint->setDriveVelocity(0);
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, false);

        std::cout << "IS CLOSED" << std::endl;
    }
    if (m_moveState == OPENING && angle >= (limit.upper - 0.01)) {
        m_moveState = OPEN;
        m_pxJoint->setDriveVelocity(0);
        m_pxJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, false);

        std::cout << "IS OPEN" << std::endl;
    }
}
