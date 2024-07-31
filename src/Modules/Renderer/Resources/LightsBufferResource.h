#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/LightComponent.h"
#include "ShaderProgramResource.h"

class LightsBufferResource : public Resource {
public:
    inline static const int MAX_SPOT_LIGHTS = 100;

    struct alignas(16) LightStructure {
        glm::vec3 color{1.0};
        float intensity{1.0};

        glm::vec3 position{0.0};
        float attenuation{1.0};

        glm::mat4 projectionViewMatrix{1.0};

        glm::vec3 direction{0.0};
        int isPointSource{1};

        std::uint64_t projectorSamplerHandle{0};
        glm::uvec2 _PLACEHOLDER1{0};

        std::uint64_t shadowSamplerHandle{0};
        float bias{0.0001};
        float _PLACEHOLDER2{0};
    };

    LightsBufferResource();

    Resource::Status build() override;
    void destroy() override;
    void bind(ShaderProgramResource &shader);
    void appendLight(TransformComponent &, LightComponent &);

    ShaderStorageBuffer<LightStructure> m_StorageBuffer;

private:
    std::string getSizeAttributeName() {
        return m_Path + "_size";
    }
    static glm::mat4 createPerspectiveProjectionViewMatrix(TransformComponent &transform, LightComponent &light);
    static glm::mat4 createPerspectiveProjectionViewMatrix(glm::vec3 direction, glm::vec3 position, float far);
    static glm::mat4 createOrthoProjectionViewMatrix(TransformComponent &transform, LightComponent &light);
};
