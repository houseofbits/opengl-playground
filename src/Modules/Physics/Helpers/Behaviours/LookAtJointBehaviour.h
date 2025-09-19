#pragma once

#include "../../../../Core/API.h"

namespace JPH {
    class Vec3;
    class Mat44;
    class Quat;
}

class PhysicsHingeJointComponent;
class PhysicsSwingTwistJointComponent;

class LookAtJointBehaviour
{
public:
    LookAtJointBehaviour();

    void serialize(nlohmann::json &j);

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager);

    void processHingeJoint(PhysicsHingeJointComponent *joint);

    void processSwingTwistJoint(PhysicsSwingTwistJointComponent *joint);

    static JPH::Quat createRotation(const JPH::Vec3& lookat, const JPH::Vec3& up);

    glm::vec3 m_targetPosition;
};
