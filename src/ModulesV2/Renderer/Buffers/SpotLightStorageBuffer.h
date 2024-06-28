#pragma once

#include "../../../Renderer/Shader/Shader.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "../../../ResourcesV2/ShaderProgramResource.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/LightComponent.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct SpotLightStructure {
    glm::vec3 color{1.0};
    float intensity{1.0};
    glm::vec3 position{0.0};
    float attenuation{1.0};
    glm::mat4 projectionViewMatrix{1.0};
    int projectorSamplerIndex{-1};
    glm::vec3 _PLACEHOLDER{0};
};

class SpotLightStorageBuffer : public ShaderStorageBuffer<SpotLightStructure> {
public:
    inline static const int MAX_SPOT_LIGHTS = 100;

    SpotLightStorageBuffer();

    void initialize();
    void appendLight(TransformComponent &, LightComponent &, int projectorIndex);
    void bind(ShaderProgramResource &);

private:
    static glm::mat4 createProjectionViewMatrix(TransformComponent &, LightComponent &);
};