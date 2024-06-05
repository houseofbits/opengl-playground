#pragma once
#include "BaseConfiguration.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <optional>
#include <string>

struct ModelBuildConfiguration {
    std::string model{};
    std::optional<glm::vec3> diffuseColor = std::nullopt;
    std::optional<float> selfIllumination = std::nullopt;
    std::optional<bool> doesReceiveShadows = std::nullopt;
    std::optional<bool> doesCastShadows = std::nullopt;
    std::optional<bool> isSolidMaterial = std::nullopt;
    std::optional<unsigned int> specularPower = std::nullopt;
    std::optional<float> roughness = std::nullopt;
    std::optional<glm::vec3> specularColor = std::nullopt;
    std::optional<std::string> diffuseTexture = std::nullopt;
    std::optional<std::string> normalTexture = std::nullopt;
    std::optional<std::string> specularTexture = std::nullopt;
    std::optional<glm::vec3> position = std::nullopt;
    std::optional<glm::vec3> rotation = std::nullopt;
    std::optional<glm::vec3> scale = std::nullopt;
};

NLOHMANN_JSONIFY_TYPE(
        ModelBuildConfiguration,
        model,
        diffuseColor,
        selfIllumination,
        doesReceiveShadows,
        doesCastShadows,
        isSolidMaterial,
        specularPower,
        roughness,
        specularColor,
        diffuseTexture,
        normalTexture,
        specularTexture,
        position,
        rotation,
        scale);