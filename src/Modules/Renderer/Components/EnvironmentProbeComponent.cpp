#include "EnvironmentProbeComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/EnvironmentProbeRenderSystem.h"
#include "../Systems/StorageBufferUpdateSystem.h"

EnvironmentProbeComponent::EnvironmentProbeComponent() : Component(), m_DebugColor(1.0), m_cubeMapLayerIndex(-1) {
}

void EnvironmentProbeComponent::serialize(nlohmann::json &j) {
    j[DEBUG_COLOR_KEY] = m_DebugColor;
}

void EnvironmentProbeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_DebugColor = j.value(DEBUG_COLOR_KEY, m_DebugColor);
}

void EnvironmentProbeComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<StorageBufferUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
}
