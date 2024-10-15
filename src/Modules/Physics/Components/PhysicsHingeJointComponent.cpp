#include "PhysicsHingeJointComponent.h"

#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/JointsProcessingSystem.h"
#include <utility>
#include "../Helpers/PhysicsTypeCast.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>

PhysicsHingeJointComponent::PhysicsHingeJointComponent() : Component(),
                                                           ComponentTransformEdit(),
                                                           BasePhysicsJoint(),
                                                           m_PhysicsResource(),
                                                           m_Joint(nullptr),
                                                           m_isLimitsEnabled(true),
                                                           m_isLockingToLimitsEnabled(true),
                                                           m_angularLimits(0, 1.0),
                                                           m_axisA(0, 1, 0),
                                                           m_axisB(0, 1, 0),
                                                           m_localAttachmentA(0, 0, 0),
                                                           m_localAttachmentB(0, 0, 0),
                                                           m_moveState(UNKNOWN) {
}

void PhysicsHingeJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
    j[ARE_LIMITS_ENABLED_KEY] = m_isLimitsEnabled;
    j[ARE_LOCKING_LIMITS_ENABLED_KEY] = m_isLockingToLimitsEnabled;
    j[AXIS_A_KEY] = m_axisA;
    j[AXIS_B_KEY] = m_axisB;
    j[ATTACHMENT_A_KEY] = m_localAttachmentA;
    j[ATTACHMENT_B_KEY] = m_localAttachmentB;
    j[LIMITS_KEY] = m_angularLimits;
}

void PhysicsHingeJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {

    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);
    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_isLockingToLimitsEnabled = j.value(ARE_LOCKING_LIMITS_ENABLED_KEY, m_isLockingToLimitsEnabled);
    m_axisA = j.value(AXIS_A_KEY, m_axisA);
    m_axisB = j.value(AXIS_B_KEY, m_axisB);
    m_localAttachmentA = j.value(ATTACHMENT_A_KEY, m_localAttachmentA);
    m_localAttachmentB = j.value(ATTACHMENT_B_KEY, m_localAttachmentB);
    m_angularLimits = j.value(LIMITS_KEY, m_angularLimits);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsHingeJointComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<JointsProcessingSystem>(this);
}

bool PhysicsHingeJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsHingeJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
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

    if (m_isLimitsEnabled) {
        settings.mLimitsMin = m_angularLimits.x;
        settings.mLimitsMax = m_angularLimits.y;
    }

    auto *groupFilter = new JPH::GroupFilterTable(2);
    groupFilter->DisableCollision(0, 1);
    bodyA.m_physicsBody->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 0));
    bodyB.m_physicsBody->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 1));

    m_Joint = (JPH::HingeConstraint *) settings.Create(*bodyA.m_physicsBody, *bodyB.m_physicsBody);

    m_PhysicsResource().getSystem().AddConstraint(m_Joint);

    bodyA.wakeUp();
    bodyB.wakeUp();
}

void PhysicsHingeJointComponent::attach(std::string entityName) {
    release();
    m_targetEntityName = std::move(entityName);
}

void PhysicsHingeJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        delete m_Joint;
        m_Joint = nullptr;
    }
}

void PhysicsHingeJointComponent::activate() {
    if (m_moveState == CLOSED) {
        open();
    }
    if (m_moveState == OPEN) {
        close();
    }
}

void PhysicsHingeJointComponent::update() {
    float angle = m_Joint->GetCurrentAngle();

    if (m_moveState != CLOSED && angle <= (m_angularLimits.x + 0.01)) {
        m_moveState = CLOSED;
        m_PhysicsResource().getInterface().SetAngularVelocity(m_Joint->GetBody1()->GetID(), {0, 0, 0});
        if (m_isLockingToLimitsEnabled) {
            m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(), JPH::EMotionType::Static,
                                                             JPH::EActivation::DontActivate);
        }
    }

    if (m_moveState != OPEN && angle >= (m_angularLimits.y - 0.01)) {
        m_moveState = OPEN;
        m_PhysicsResource().getInterface().SetAngularVelocity(m_Joint->GetBody1()->GetID(), {0, 0, 0});
        if (m_isLockingToLimitsEnabled) {
            m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(), JPH::EMotionType::Static,
                                                             JPH::EActivation::DontActivate);
        }
    }
}

bool PhysicsHingeJointComponent::isCreated() const {
    return m_Joint != nullptr;
}

void PhysicsHingeJointComponent::open() {
    m_moveState = OPENING;
    if (m_isLockingToLimitsEnabled) {
        m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(), JPH::EMotionType::Dynamic,
                                                         JPH::EActivation::Activate);
    }

    m_PhysicsResource().getInterface().SetAngularVelocity(m_Joint->GetBody1()->GetID(), {0, -10, 0});

    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}

void PhysicsHingeJointComponent::close() {
    m_moveState = CLOSING;
    if (m_isLockingToLimitsEnabled) {
        m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(), JPH::EMotionType::Dynamic,
                                                         JPH::EActivation::Activate);
    }

    m_PhysicsResource().getInterface().SetAngularVelocity(m_Joint->GetBody1()->GetID(), {0, 10, 0});

    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}

glm::mat4 PhysicsHingeJointComponent::getEditorTransform() {
    return glm::mat4(1.0);
}

void PhysicsHingeJointComponent::setFromEditorTransform(const glm::mat4 &) {

}
