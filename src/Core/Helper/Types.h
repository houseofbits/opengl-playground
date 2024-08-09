#pragma once

#include <PhysX/foundation/PxVec3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace Types {

    physx::PxVec3 GLMtoPxVec3(const glm::vec3& val) {
        return {val.x, val.y, val.z};
    }

    physx::PxExtendedVec3 GLMtoPxExtendedVec3(const glm::vec3& val) {
        return {val.x, val.y, val.z};
    }

    physx::PxTransform GLMtoPxTransform(const glm::mat4& m) {
        glm::quat rq = glm::quat_cast(m);
        physx::PxQuat rotation(rq.x, rq.y, rq.z, rq.w);
        physx::PxTransform transform(m[3].x, m[3].y, m[3].z, rotation);

        return transform;
    }
}