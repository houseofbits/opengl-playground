#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "ShaderProgramResource.h"
#include "RenderShaderResource.h"

class EnvironmentProbesBufferResource : public RenderShaderResource {
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

    void applyToShader(RenderShader &shader) override;

    ShaderStorageBuffer<EnvironmentProbeStructure> m_StorageBuffer;

private:
    std::string getSizeAttributeName() {
        return m_Path + "_size";
    }
};
