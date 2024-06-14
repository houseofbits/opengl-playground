#pragma once
#include "BaseConfiguration.h"
#include <glm/vec3.hpp>
#include <optional>
#include <string>

struct ProbeBuildConfiguration {
    glm::vec3 position{0, 0, 0};
    glm::vec3 boundingBoxMin{0, 0, 0};
    glm::vec3 boundingBoxMax{0, 0, 0};
};

//NLOHMANN_JSONIFY_TYPE(
//        ProbeBuildConfiguration,
//        position,
//        boundingBoxMin,
//        boundingBoxMax);