#include "SkyComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/EnvironmentProbeRenderSystem.h"
#include "../Systems/MainRenderSystem.h"

SkyComponent::SkyComponent() : Component(), m_cubeMap() {
}

void SkyComponent::registerWithSystems(EntityContext& ctx) {

}