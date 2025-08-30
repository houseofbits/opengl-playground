#include "MeshComponent.h"

MeshComponent::MeshComponent() : Component(), m_Mesh(), m_Material(), m_shouldOverrideMaterial(false) {
}

void MeshComponent::serialize(nlohmann::json &j) {
    j[MODEL_KEY] = m_Mesh().m_Path;

    if (m_shouldOverrideMaterial) {
        j[OVERRIDE_MATERIAL_KEY] = m_Material().m_Path;
    }
}

void MeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    std::string path = j.value(MODEL_KEY, m_Mesh().m_Path);
    resourceManager.request(m_Mesh, path);

    if (j.contains(OVERRIDE_MATERIAL_KEY)) {
        m_shouldOverrideMaterial = true;

        path = j.value(OVERRIDE_MATERIAL_KEY, m_Material().m_Path);
        resourceManager.request(m_Material, path);
    }
}

void MeshComponent::setNodeMaterial(const std::string &nodeName, const std::string &materialName) {
}

std::string MeshComponent::getNodeMaterial(const std::string &nodeName) {
    return "";
}

void MeshComponent::render(const glm::mat4 &worldTransform, ShaderProgramResource &shader,
                           MaterialResource &defaultMaterial) {
    if (m_shouldOverrideMaterial && m_Material.isReady()) {
        m_Material().bind(shader);
        m_Mesh().render(worldTransform, shader, true, defaultMaterial);
    } else {
        m_Mesh().render(worldTransform, shader, false, defaultMaterial);
    }
}
