#include "MaterialResource.h"
#include <fstream>
#include "../../../Renderer/Material/MaterialConfigurationGLTFLoader.h"
#include "../../../Renderer/Material/MaterialConfigurationSerializer.h"

MaterialResource::MaterialResource() : Resource(),
                                       m_materialConfiguration(),
                                       m_Diffuse(),
                                       m_Normal(),
                                       m_Roughness(),
                                       m_Emissive(),
                                       m_materialBufferIndex(0) {
}

void MaterialResource::destroy() {
    m_Diffuse.invalidate();
    m_Normal.invalidate();
    m_Roughness.invalidate();
}

void MaterialResource::bind(ShaderProgramResource &shader) {
    shader.setUniform("primaryMaterialIndex", m_materialBufferIndex);
}

void MaterialResource::fetchFromGLTF(ResourceManager &resourceManager, const tinygltf::Model &model,
                                     const tinygltf::Material &material,
                                     const std::string &basePath) {
    m_materialConfiguration = MaterialConfigurationGLTFLoader::createFromGLTFMaterial(model, material, basePath);

    requestTextureResource(resourceManager, m_Diffuse, m_materialConfiguration.diffuseTextureUri);
    requestTextureResource(resourceManager, m_Normal, m_materialConfiguration.normalTextureUri);
    requestTextureResource(resourceManager, m_Roughness, m_materialConfiguration.roughnessMetallicTextureUri);
    requestTextureResource(resourceManager, m_Emissive, m_materialConfiguration.emissiveTextureUri);

    setDataReady();
}

void MaterialResource::requestTextureResource(ResourceManager &resourceManager,
                                              ResourceHandle<TextureResource> &resource,
                                              const std::string &fileUri) {
    auto uri = fileUri;

    if (fileUri.empty()) {
        return;
    }

    addDependency(uri);
    resourceManager.request(resource, uri);
}

void MaterialResource::fetchDefault(ResourceManager &resourceManager) {
    m_materialConfiguration.diffuseTextureUri = "resources/textures/grid-gray.png";
    m_materialConfiguration.diffuseTextureWrappingType = MaterialConfiguration::WRAPPING_TRIPLANAR;

    addDependency(m_materialConfiguration.diffuseTextureUri);
    resourceManager.request(m_Diffuse, m_materialConfiguration.diffuseTextureUri);

    setDataReady();
}

Resource::Status MaterialResource::fetchData(ResourceManager &resourceManager) {
    auto json = Json::readFile(getFullPath());
    if (!json) {
        return STATUS_FETCH_ERROR;
    }

    m_materialConfiguration = MaterialConfigurationSerializer::deserialize(json.value());

    requestTextureResource(resourceManager, m_Diffuse, m_materialConfiguration.diffuseTextureUri);
    requestTextureResource(resourceManager, m_Normal, m_materialConfiguration.normalTextureUri);
    requestTextureResource(resourceManager, m_Roughness, m_materialConfiguration.roughnessMetallicTextureUri);
    requestTextureResource(resourceManager, m_Emissive, m_materialConfiguration.emissiveTextureUri);

    return STATUS_DATA_READY;
}

Resource::Status MaterialResource::build() {
    return STATUS_READY;
}

std::string MaterialResource::getFullPath() const {
    return getFullPath(m_Path);
}
