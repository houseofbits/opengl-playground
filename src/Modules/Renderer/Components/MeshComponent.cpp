#include "MeshComponent.h"

MeshComponent::MeshComponent() : Component(), m_Mesh(), m_Material() {
}

void MeshComponent::serialize(nlohmann::json &j) {
    j[MODEL_KEY] = m_Mesh().m_Path;
    j["material"] = m_Material().m_Path;
}

void MeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    std::string path = j.value(MODEL_KEY, m_Mesh().m_Path);
    resourceManager.request(m_Mesh, path);

    path = j.value("material", m_Material().m_Path);
    resourceManager.request(m_Material, path);
}