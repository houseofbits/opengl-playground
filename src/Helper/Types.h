#pragma once

#include <PhysX/foundation/PxVec3.h>
#include <glm/vec3.hpp>

namespace Types {

    physx::PxVec3 GLMtoPX(const glm::vec3& val) {
        return {val.x, val.y, val.z};
    }

}