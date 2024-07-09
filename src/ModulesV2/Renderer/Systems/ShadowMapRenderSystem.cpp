#include "ShadowMapRenderSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/StaticMeshComponent.h"

ShadowMapRenderSystem::ShadowMapRenderSystem() : EntitySystem(),
                                                 m_LightsBuffer(),
                                                 m_ShadowAtlas() {
    usesComponent<StaticMeshComponent>();
    usesComponent<TransformComponent>();
}

void ShadowMapRenderSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_ShadowAtlas, "ShadowAtlas");
}

void ShadowMapRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &ShadowMapRenderSystem::handleEditorUIEvent);
}

bool ShadowMapRenderSystem::handleEditorUIEvent(EditorUIEvent *event) {
    return true;
}

void ShadowMapRenderSystem::process() {
}