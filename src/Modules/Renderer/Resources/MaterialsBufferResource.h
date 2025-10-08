#pragma once

#include "MaterialResource.h"
#include "RenderShaderResource.h"
#include "../../../Core/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"

class MaterialsBufferResource : public RenderShaderResource {
public:
    inline static const int MAX_MATERIALS = 100;

    MaterialsBufferResource();

    struct alignas(16) MaterialStructure {
        glm::vec4 diffuseColor{1.0};
        glm::vec3 emissiveColor{0.0};
        float selfIllumination{0};
        std::uint64_t diffuseTextureHandle{0};
        std::uint64_t normalTextureHandle{0};
        std::uint64_t roughnessTextureHandle{0};
        std::uint64_t emissiveTextureHandle{0};

        float roughnessFactor{1.0};
        float metallicFactor{0.0};
        int doesCastShadows{true};
        int doesReceiveShadows{true};

        int doesUseTriplanarMapping{false};
        int _PLACEHOLDER1{0};
        int _PLACEHOLDER2{0};
        int _PLACEHOLDER3{0};
    };

    ShaderStorageBuffer<MaterialStructure> m_StorageBuffer;

    void appendMaterial(MaterialResource &material);

    Status build() override;

    void destroy() override;

    void bind(ShaderProgramResource &shader);

    void applyToShader(RenderShader &shader) override;

    std::string getSizeAttributeName() {
        return m_Path + "_size";
    }
};
