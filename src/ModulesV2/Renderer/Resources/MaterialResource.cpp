#include "MaterialResource.h"
#include <fstream>

MaterialResource::MaterialResource() : Resource(), m_Diffuse(), m_Normal() {
}

Resource::Status MaterialResource::fetchData(ResourceManager &manager) {
    std::ifstream file(m_Path);
    if (file.fail()) {
        return STATUS_FETCH_ERROR;
    }
    auto json = nlohmann::json::parse(file);

    if (json.contains(DIFFUSE_TEXTURE_KEY)) {
        manager.request(m_Diffuse, json[DIFFUSE_TEXTURE_KEY]);
    }

    if (json.contains(NORMAL_TEXTURE_KEY)) {
        manager.request(m_Normal, json[NORMAL_TEXTURE_KEY]);
    }

    return STATUS_DATA_READY;
}

Resource::Status MaterialResource::build() {
    if (m_Diffuse.isValid() && !m_Diffuse.m_Resource->isFinished()) {
        return STATUS_BUILDING;
    }

    if (m_Normal.isValid() && !m_Normal.m_Resource->isFinished()) {
        return STATUS_BUILDING;
    }

    return STATUS_READY;
}

void MaterialResource::destroy() {
    m_Diffuse.invalidate();
    m_Normal.invalidate();
}

void MaterialResource::bind(ShaderProgramResource &shader) {
    if (m_Diffuse().isReady()) {
        shader.setUniform("diffuseSampler", m_Diffuse().m_handleId);
    }

    if (m_Normal().isReady()) {
        shader.setUniform("normalSampler", m_Normal().m_handleId);
    }
}
