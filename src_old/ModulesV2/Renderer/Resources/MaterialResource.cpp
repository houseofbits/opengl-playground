#include "MaterialResource.h"
#include <fstream>

MaterialResource::MaterialResource() : Resource(), m_Diffuse(), m_Normal(), m_Roughness() {
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

    shader.setUniform("hasDiffuseSampler", (int)m_Diffuse().isReady());
    shader.setUniform("hasNormalSampler", (int)m_Normal().isReady());
    shader.setUniform("hasRoughnessSampler", (int)m_Roughness().isReady());
}
