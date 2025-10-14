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
                   static_cast<int>(component->m_renderTarget().m_width),
                   static_cast<int>(component->m_renderTarget().m_height));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

        m_mainRenderQueue().render(component->m_Camera);

        RenderTargetResource::unbindRenderTarget();
    }
}

void CameraRenderTargetSystem::registerEventHandlers(EventManager &eventManager) {
}
