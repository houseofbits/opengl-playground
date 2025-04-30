#include "MaterialResource.h"
#include <fstream>
#include "../../../Renderer/Material/MaterialConfigurationGLTFLoader.h"

MaterialResource::MaterialResource() : Resource(),
                                       m_materialConfiguration(),
                                       m_Diffuse(),
                                       m_Normal(),
                                       m_Roughness() {
}

Resource::Status MaterialResource::build() {
    return STATUS_READY;
}

void MaterialResource::destroy() {
    m_Diffuse.invalidate();
    m_Normal.invalidate();
    m_Roughness.invalidate();
}

void MaterialResource::bind(ShaderProgramResource &shader) {
    if (m_Diffuse().isReady()) {
        shader.setUniform("diffuseSampler", m_Diffuse().m_handleId);
    }

    if (m_Normal().isReady()) {
        shader.setUniform("normalSampler", m_Normal().m_handleId);
    }

    if (m_Roughness().isReady()) {
        shader.setUniform("roughnessSampler", m_Roughness().m_handleId);
    }

    if (m_Emissive().isReady()) {
        shader.setUniform("emissiveSampler", m_Emissive().m_handleId);
    }

    shader.setUniform("diffuseColor", m_materialConfiguration.diffuseColor);
    shader.setUniform("emissiveColor", m_materialConfiguration.emissiveColor);
    shader.setUniform("selfIllumination", m_materialConfiguration.selfIllumination);
    shader.setUniform("hasDiffuseSampler", static_cast<int>(m_Diffuse().isReady()));
    shader.setUniform("hasNormalSampler", static_cast<int>(m_Normal().isReady()));
    shader.setUniform("hasRoughnessSampler", static_cast<int>(m_Roughness().isReady()));
    shader.setUniform("hasEmissivesSampler", static_cast<int>(m_Emissive().isReady()));
    shader.setUniform("doesReceiveShadows", static_cast<int>(m_materialConfiguration.doesReceiveShadows));
    shader.setUniform("wrappingType", (int) m_materialConfiguration.diffuseTextureWrappingType);
}

void MaterialResource::fetchFromGLTF(ResourceManager &resourceManager, const tinygltf::Model &model,
                                     const tinygltf::Material &material) {
    m_materialConfiguration = MaterialConfigurationGLTFLoader::createFromGLTFMaterial(model, material);

    requestTextureResource(resourceManager, m_Diffuse, m_materialConfiguration.diffuseTextureUri);
    requestTextureResource(resourceManager, m_Normal, m_materialConfiguration.normalTextureUri);
    requestTextureResource(resourceManager, m_Roughness, m_materialConfiguration.roughnessMetallicTextureUri);
    requestTextureResource(resourceManager, m_Emissive, m_materialConfiguration.emissiveTextureUri);

    setDataReady();
}

void MaterialResource::requestTextureResource(ResourceManager &resourceManager,
                                              ResourceHandle<TextureResource> &resource, const std::string &fileUri) {
    if (fileUri.empty()) {
        return;
    }

    addDependency(fileUri);
    resourceManager.request(resource, fileUri);
}

void MaterialResource::fetchDefault(ResourceManager &resourceManager) {
    m_materialConfiguration.diffuseTextureUri = "resources/textures/grid-gray.png";
    m_materialConfiguration.diffuseTextureWrappingType = MaterialConfiguration::WRAPPING_TRIPLANAR;

    addDependency(m_materialConfiguration.diffuseTextureUri);
    resourceManager.request(m_Diffuse, m_materialConfiguration.diffuseTextureUri);

    setDataReady();
}
