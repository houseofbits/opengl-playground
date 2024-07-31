#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "ShaderProgramResource.h"

class EnvironmentProbesBufferResource : public Resource {
public:
    inline static const int MAX_PROBES = 100;

    struct EnvironmentProbeStructure {
        glm::vec3 position{0};
        int cubeMapTextureLayer{-1};
        glm::vec4 boundingBoxMin{-0.5};
        glm::vec4 boundingBoxMax{0.5};
        glm::vec3 debugColor{1};
        float _PLACEHOLDER0{0};
    };

    EnvironmentProbesBufferResource();

    Resource::Status build() override;
    void destroy() override;
    void bind(ShaderProgramResource &shader);
    void appendProbe(TransformComponent &transform, EnvironmentProbeComponent &probe);

    ShaderStorageBuffer<EnvironmentProbeStructure> m_StorageBuffer;

private:
    std::string getSizeAttributeName() {
        return m_Path + "_size";
    }
};
