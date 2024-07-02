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

    glm::vec3 direction{0.0};
    int projectorSamplerIndex{-1};

    int isPointSource{1};
    float _PLACEHOLDER0{0};
    float _PLACEHOLDER1{0};
    float _PLACEHOLDER2{0};
};

class LightStorageBuffer : public ShaderStorageBuffer<SpotLightStructure> {
public:
    inline static const int MAX_SPOT_LIGHTS = 100;

    LightStorageBuffer();

    void initialize();
    void appendLight(TransformComponent &, LightComponent &, int projectorIndex);
    void bind(ShaderProgramResource &);

private:
    static glm::mat4 createPerspectiveProjectionViewMatrix(TransformComponent &transform, LightComponent &light);
    static glm::mat4 createPerspectiveProjectionViewMatrix(glm::vec3 direction, glm::vec3 position, float far);
    static glm::mat4 createOrthoProjectionViewMatrix(TransformComponent &transform, LightComponent &light);
};