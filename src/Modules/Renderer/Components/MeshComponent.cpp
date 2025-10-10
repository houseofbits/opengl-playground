#include "MeshComponent.h"

MeshComponent::MeshComponent() : Component(), m_Mesh(), m_Material(), m_shouldOverrideMaterial(false),
                                 m_isVisible(true) {
}

void MeshComponent::serialize(nlohmann::json &j) {
    j[MODEL_KEY] = m_Mesh().m_Path;

    if (m_shouldOverrideMaterial) {
        j[OVERRIDE_MATERIAL_KEY] = m_Material().m_Path;
    }

    j[IS_VISIBLE_KEY] = m_isVisible;
}

void MeshComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    std::string path = j.value(MODEL_KEY, m_Mesh().m_Path);
    resourceManager.request(m_Mesh, path);

    if (j.contains(OVERRIDE_MATERIAL_KEY)) {
        m_shouldOverrideMaterial = true;

        path = j.value(OVERRIDE_MATERIAL_KEY, m_Material().m_Path);
        resourceManager.request(m_Material, path);
    }

    m_isVisible = j.value(IS_VISIBLE_KEY, m_isVisible);
}

void MeshComponent::setNodeMaterial(const std::string &nodeName, const std::string &materialName) {
}

std::string MeshComponent::getNodeMaterial(const std::string &nodeName) {
    return "";
}

void MeshComponent::render(const glm::mat4 &worldTransform, ShaderProgramResource &shader,
                           MaterialResource &defaultMaterial) {
    if (!m_isVisible) {
        return;
    }

    if (m_shouldOverrideMaterial && m_Material.isReady()) {
        m_Material().bind(shader);
        m_Mesh().render(worldTransform, shader, true, defaultMaterial);
    } else {
        m_Mesh().render(worldTransform, shader, false, defaultMaterial);
    }
}

void MeshComponent::addToRenderQueue(RenderQueue &queue, RenderShader &shader, const glm::mat4 &worldTransform,
                                     const MaterialResource &defaultMaterial) {
    for (const auto &mesh: m_Mesh().m_model.m_meshNodes) {
        RenderQueueItem item;
        item.shader = &shader;
        item.vertexArray = m_Mesh().m_model.m_vertexArrayObject;
        item.offset = mesh.offset;
        item.size = mesh.size;
        item.modelMatrix = mesh.modelMatrix * worldTransform;
        item.materialIndex = 1;

        if (m_shouldOverrideMaterial && m_Material().isReady()) {
            item.materialIndex = m_Material().m_materialBufferIndex;
        } else  if (mesh.materialIndex >= 0 && m_Mesh().m_materials[mesh.materialIndex].isReady()) {
            item.materialIndex = m_Mesh().m_materials[mesh.materialIndex]().m_materialBufferIndex;
        } else {
            item.materialIndex = defaultMaterial.m_materialBufferIndex;
        }

        queue.add(item);
    }
}
