#include "MaterialsBufferResource.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"

MaterialsBufferResource::MaterialsBufferResource() : RenderShaderResource(), m_StorageBuffer() {
}

Resource::Status MaterialsBufferResource::build() {
    if (!ShaderStorageBuffer<MaterialsBufferResource::MaterialStructure>::isSupported()) {
        return STATUS_BUILD_ERROR;
    }

    m_StorageBuffer.create(
        MAX_MATERIALS,
        ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void MaterialsBufferResource::appendMaterial(MaterialResource &material) {
    MaterialStructure structure;

    structure.diffuseColor = glm::vec4(material.m_materialConfiguration.diffuseColor, 1.0);
    structure.emissiveColor = material.m_materialConfiguration.emissiveColor;
    structure.selfIllumination = material.m_materialConfiguration.selfIllumination;
    structure.roughnessFactor = material.m_materialConfiguration.roughnessFactor;
    structure.metallicFactor = material.m_materialConfiguration.metallicFactor;
    structure.doesCastShadows = material.m_materialConfiguration.doesCastShadows;
    structure.doesReceiveShadows = material.m_materialConfiguration.doesReceiveShadows;

    if (material.m_Diffuse().isReady()) {
        structure.diffuseTextureHandle = material.m_Diffuse().m_handleId;
    }
    if (material.m_Normal().isReady()) {
        structure.normalTextureHandle = material.m_Normal().m_handleId;
    }
    if (material.m_Roughness().isReady()) {
        structure.roughnessTextureHandle = material.m_Roughness().m_handleId;
    }
    if (material.m_Emissive().isReady()) {
        structure.emissiveTextureHandle = material.m_Emissive().m_handleId;
    }

    structure.doesUseTriplanarMapping = material.m_materialConfiguration.diffuseTextureWrappingType ==
                                        MaterialConfiguration::WRAPPING_TRIPLANAR;

    material.m_materialBufferIndex = static_cast<int>(m_StorageBuffer.currentSize);

    m_StorageBuffer.append(structure);
}

void MaterialsBufferResource::destroy() {
}

void MaterialsBufferResource::bind(ShaderProgramResource &shader) {
    m_StorageBuffer.bind();
    shader.setUniform(getSizeAttributeName().c_str(), m_StorageBuffer.currentSize);
}

void MaterialsBufferResource::applyToShader(RenderShader &shader) {
    shader.addStorageBuffer(&m_StorageBuffer);
    shader.setUniform(getSizeAttributeName().c_str(), m_StorageBuffer.currentSize);
}
