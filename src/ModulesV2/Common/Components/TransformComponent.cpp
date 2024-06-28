#include "TransformComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../../Renderer/Systems/RendererSystem.h"

TransformComponent::TransformComponent() : m_ModelMatrix(1.0), m_Translation(0.0), m_Rotation(1, 0, 0, 0), m_Scale(1.0) {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void TransformComponent::decomposeModelMatrix(glm::vec3 &translation, glm::quat &rotation, glm::vec3 &scale) {
    translation = m_ModelMatrix[3];

    for (int i = 0; i < 3; i++)
        scale[i] = glm::length(glm::vec3(m_ModelMatrix[i]));

    const glm::mat3 rotMtx(
            glm::vec3(m_ModelMatrix[0]) / scale[0],
            glm::vec3(m_ModelMatrix[1]) / scale[1],
            glm::vec3(m_ModelMatrix[2]) / scale[2]);

    rotation = glm::quat_cast(rotMtx);
}
