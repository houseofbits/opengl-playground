#include "CameraRenderTargetSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Resources/RenderQueueResource.h"

CameraRenderTargetSystem::CameraRenderTargetSystem() : EntitySystem(), m_cameraComponentRegistry() {
    m_cameraComponentRegistry = useEntityUniqueComponentRegistry<CameraComponent>();
}

void CameraRenderTargetSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_mainRenderQueue, "mainRenderQueue");
}

void CameraRenderTargetSystem::process(EventManager &) {
    for (const auto &[entityId, component]: m_cameraComponentRegistry->container()) {
        if (!component->m_isRenderTargetEnabled || !component->m_renderTarget.isReady()) {
            continue;
        }

        component->m_renderTarget().bindRenderTarget();
        glViewport(0,
                   0,
                   component->m_renderTarget().m_width,
                   component->m_renderTarget().m_height);

//        Log::write("Render target camera ", component->getRenderTargetResourceName());
        m_mainRenderQueue().render(component->m_Camera);

        RenderTargetResource::unbindRenderTarget();
    }
}

void CameraRenderTargetSystem::registerEventHandlers(EventManager &eventManager) {
}
