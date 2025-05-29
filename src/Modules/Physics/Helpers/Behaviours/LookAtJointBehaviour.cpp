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

// void printQuatAxis(JPH::Quat q) {
//     auto x = q * JPH::Vec3::sAxisX();
//     auto y = q * JPH::Vec3::sAxisY();
//     auto z = q * JPH::Vec3::sAxisZ();
//
//     Log::write(x, " | ", y, " | ", z);
// }

//
// JPH::Quat quatFromAxis(JPH::Vec3 x, JPH::Vec3 y) {
//     auto z = x.Cross(y).Normalized();
//     auto yN = z.Cross(x).Normalized();
//
//     JPH::Mat44 rotation = JPH::Mat44::sIdentity();
//
//     rotation.SetAxisX(x.Normalized());
//     rotation.SetAxisY(y);
//     rotation.SetAxisZ(z);
//
//     return rotation.GetQuaternion();
// }
//
// JPH::Quat lookAt(JPH::Vec3 lookat, JPH::Vec3 up) {
//     auto z = up.Cross(lookat).Normalized();
//     auto x = lookat.Cross(z).Normalized();
//
//     JPH::Mat44 rotation = JPH::Mat44::sIdentity();
//
//     rotation.SetAxisX(x);
//     rotation.SetAxisY(lookat.Normalized());
//     rotation.SetAxisZ(z);
//
//     return rotation.GetQuaternion();
// }

void LookAtJointBehaviour::processSwingTwistJoint(PhysicsSwingTwistJointComponent *jointComponent) {
    const auto &joint = jointComponent->m_Joint;

    const auto pivot = joint->GetBody2()->GetWorldTransform() * joint->GetLocalSpacePosition2();
    auto targetDirection = (PhysicsTypeCast::glmToJPH(m_targetPosition) - pivot).Normalized();

    const auto constrB2World = joint->GetBody2()->GetRotation() * joint->GetConstraintToBody2();
    auto vConstrSpace = constrB2World.Conjugated() * targetDirection;

    auto r = createRotation(vConstrSpace.Normalized(), JPH::Vec3(0,1,0));

    joint->SetSwingMotorState(JPH::EMotorState::Position);
    joint->SetTargetOrientationCS(r.Conjugated());
}

JPH::Quat LookAtJointBehaviour::createRotation(const JPH::Vec3 &lookat, const JPH::Vec3 &up) {
    auto z = lookat.Cross(up).Normalized();
    auto y = z.Cross(lookat).Normalized();

    JPH::Mat44 rotation = JPH::Mat44::sIdentity();

    rotation.SetAxisX(lookat.Normalized());
    rotation.SetAxisY(y);
    rotation.SetAxisZ(z);

    return rotation.GetQuaternion().Normalized();
}
