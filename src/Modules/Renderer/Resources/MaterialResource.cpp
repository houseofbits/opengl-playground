#include "MaterialResource.h"
#include <fstream>

MaterialResource::MaterialResource() : Resource(),
                                       m_DiffuseColor(1.0),
                                       m_Diffuse(),
                                       m_Normal(),
                                       m_Roughness(),
                                       m_doesCastShadows(true),
                                       m_doesReceiveShadows(true),
                                       m_selfIllumination(0.0),
                                       m_textureWrappingType(WRAPPING_UV1) {
}

Resource::Status MaterialResource::fetchData(ResourceManager &manager) {
    std::ifstream file(m_Path);
    if (file.fail()) {
        return STATUS_FETCH_ERROR;
    }
    auto json = nlohmann::json::parse(file);

    if (json.contains(DIFFUSE_TEXTURE_KEY)) {
        addDependency(json[DIFFUSE_TEXTURE_KEY]);
        manager.request(m_Diffuse, json[DIFFUSE_TEXTURE_KEY]);
    }

    if (json.contains(NORMAL_TEXTURE_KEY)) {
        addDependency(json[NORMAL_TEXTURE_KEY]);
        manager.request(m_Normal, json[NORMAL_TEXTURE_KEY]);
    }

    if (json.contains(ROUGHNESS_TEXTURE_KEY)) {
        addDependency(json[ROUGHNESS_TEXTURE_KEY]);
        manager.request(m_Roughness, json[ROUGHNESS_TEXTURE_KEY]);
    }

    m_doesReceiveShadows = json.value(RECEIVE_SHADOWS_KEY, true);
    m_doesCastShadows = json.value(CAST_SHADOWS_KEY, true);
    m_DiffuseColor = json.value(DIFFUSE_COLOR_KEY, glm::vec3(1.0));
    m_selfIllumination = json.value(SELF_ILLUMINATION_KEY, 0.0f);
    m_textureWrappingType = json.value(WRAPPING_TYPE_KEY, m_textureWrappingType);

    return STATUS_DATA_READY;
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

    shader.setUniform("diffuseColor", m_DiffuseColor);
    shader.setUniform("selfIllumination", m_selfIllumination);
    shader.setUniform("hasDiffuseSampler", (int) m_Diffuse().isReady());
    shader.setUniform("hasNormalSampler", (int) m_Normal().isReady());
    shader.setUniform("hasRoughnessSampler", (int) m_Roughness().isReady());
    shader.setUniform("doesReceiveShadows", (int) m_doesReceiveShadows);
    shader.setUniform("wrappingType", (int) m_textureWrappingType);
}

void MaterialResource::write() {
    auto json = nlohmann::json();

    if (m_Diffuse.isReady()) {
        json[DIFFUSE_TEXTURE_KEY] = m_Diffuse().m_Path;
    }
    if (m_Roughness.isReady()) {
        json[ROUGHNESS_TEXTURE_KEY] = m_Roughness().m_Path;
    }
    if (m_Normal.isReady()) {
        json[NORMAL_TEXTURE_KEY] = m_Normal().m_Path;
    }

    json[CAST_SHADOWS_KEY] = m_doesCastShadows;
    json[RECEIVE_SHADOWS_KEY] = m_doesReceiveShadows;
    json[DIFFUSE_COLOR_KEY] = m_DiffuseColor;
    json[SELF_ILLUMINATION_KEY] = m_selfIllumination;
    json[WRAPPING_TYPE_KEY] = m_textureWrappingType;

    std::ofstream file;
    file.open(m_Path);

    std::string data(json.dump(4));
    file << data;
}
