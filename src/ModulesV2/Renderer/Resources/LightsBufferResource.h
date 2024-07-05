#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"

class LightsBufferResource : public Resource {
public:
    inline static const int MAX_SPOT_LIGHTS = 100;

    struct LightStructure {
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

    LightsBufferResource();

    Resource::Status build() override;
    void destroy() override;

    ShaderStorageBuffer<LightStructure> m_StorageBuffer;
};
