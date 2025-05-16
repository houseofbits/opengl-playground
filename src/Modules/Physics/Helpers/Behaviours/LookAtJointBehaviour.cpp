#include "LookAtJointBehaviour.h"
#include "../BasePhysicsJoint.h"
#include "../PhysicsTypeCast.h"
#include "../../Components/PhysicsSwingTwistJointComponent.h"
#include "../../Components/PhysicsHingeJointComponent.h"

LookAtJointBehaviour::LookAtJointBehaviour() : m_targetPosition(0.0) {
}

void LookAtJointBehaviour::serialize(nlohmann::json &j) {
}

void LookAtJointBehaviour::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
}

void LookAtJointBehaviour::processHingeJoint(PhysicsHingeJointComponent *joint) {
}

void LookAtJointBehaviour::processSwingTwistJoint(PhysicsSwingTwistJointComponent *jointComponent) {
    const auto &joint = jointComponent->m_Joint;

    auto targetDirection = PhysicsTypeCast::glmToJPH(m_targetPosition) - joint->GetBody2()->GetPosition();

    // Log::write(targetDirection.Normalized());

    // auto worldRotationToTarget = JPH::Quat::sRotation(JPH::Vec3Arg(0, 0, 1), JPH::DegreesToRadians(-45.0f));
    // auto worldRotationToTarget = createRotation(JPH::Vec3(1,-1,0).Normalized(), joint->GetBody2()->GetCenterOfMassTransform());
    auto worldRotationToTarget = createRotation(targetDirection.Normalized(), joint->GetBody2()->GetCenterOfMassTransform());

    // auto local = joint->GetBody2()->GetRotation().Conjugated() * worldRotationToTarget;

    JPH::Quat cBody1World = worldRotationToTarget * joint->GetConstraintToBody1();
    JPH::Quat cBody2World = joint->GetBody2()->GetRotation() * joint->GetConstraintToBody2();
    auto rt = cBody1World.Conjugated() * cBody2World;

    // Log::write(worldRotationToTarget, " / ", rt, " | ", local);
    //
    // auto rcs = joint->GetRotationInConstraintSpace();
    //
    // Log::write(rcs);
    //


    rt = JPH::Quat::sRotation(JPH::Vec3Arg(0, 1, 0), JPH::DegreesToRadians(10.0f));
    //
    // joint->SetSwingMotorState(JPH::EMotorState::Position);
    // joint->SetTargetOrientationCS(rt);
}

JPH::Quat LookAtJointBehaviour::createRotation(const JPH::Vec3 direction, const JPH::Mat44 &frame) {

    return JPH::Quat::sFromTo(JPH::Vec3(0,-1,0), direction);

    const auto lookAtMatrix = glm::lookAt(glm::vec3(0), PhysicsTypeCast::JPHToGlm(direction), glm::vec3(0, 1, 0));

    // const auto body2Z = frame.GetAxisZ();
    // const auto Y = body2Z.Cross(xDirection).Normalized();
    // const auto Z = xDirection.Cross(Y).Normalized();
    //
    // const auto rotationMatrix = JPH::Mat44(
    //     JPH::Vec4Arg(xDirection.Normalized(), 1.0),
    //     JPH::Vec4Arg(Y, 1.0),
    //     JPH::Vec4Arg(Z, 1.0), JPH::Vec3::sZero());

    glm::quat q = glm::quat_cast(lookAtMatrix);

    // Log::write(q);

    return PhysicsTypeCast::glmToJPH(q);

    // return rotationMatrix.GetQuaternion();
}
