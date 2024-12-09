#include "ShadowMapRenderSystem.h"

ShadowMapRenderSystem::ShadowMapRenderSystem() : EntitySystem(),
                                                 m_ResourceManager(nullptr),
                                                 m_LightsBuffer(),
                                                 m_ShaderProgram() {
    m_meshComponentRegistry = useComponentsRegistry<TransformComponent, StaticMeshComponent>();
    m_lightComponentRegistry = useComponentsRegistry<TransformComponent, LightComponent>();
}

void ShadowMapRenderSystem::initialize(ResourceManager &resourceManager) {
    m_ResourceManager = &resourceManager;
    resourceManager.request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager.request(m_ShaderProgram, "data/shaders/shadowMap|.vert|.frag", {"SpotLightStorageBuffer"});
}

void ShadowMapRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &ShadowMapRenderSystem::handleEditorUIEvent);
}

void ShadowMapRenderSystem::handleEditorUIEvent(const EditorUIEvent & event) {
}

void ShadowMapRenderSystem::process(EventManager &eventManager) {

    prepareShadowMapResources();

    m_ShaderProgram().use();
    m_LightsBuffer().bind(m_ShaderProgram());

    for (const auto &[id, components]: m_lightComponentRegistry->container()) {
        const auto &[transform, light] = components.get();

        if (!light->m_doesCastShadows || !light->m_isEnabled) {
            continue;
        }

        int i = 0;
        for (const auto &index: light->m_lightBufferIndices) {
            if (!light->m_ShadowMaps[i]->get().isReady()) {
                continue;
            }
            light->m_ShadowMaps[i]->get().bindRenderTarget();
            glViewport(0,
                       0,
                       (int) light->m_ShadowMaps[i]->get().m_textureRenderTarget.width,
                       (int) light->m_ShadowMaps[i]->get().m_textureRenderTarget.height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderGeometry(index);

            light->m_ShadowMaps[i]->get().unbindRenderTarget();
            i++;
        }
    }
}

void ShadowMapRenderSystem::renderGeometry(int lightIndex) {
    m_ShaderProgram().setUniform("lightIndex", lightIndex);
    for (const auto &[id, components]: m_meshComponentRegistry->container()) {
        const auto &[transform, mesh] = components.get();
        if (mesh->m_Material.isReady() && mesh->m_Material().m_doesCastShadows) {
            m_ShaderProgram().setUniform("modelMatrix", transform->getModelMatrix());
            mesh->m_Mesh().render();
        }
    }
}

void ShadowMapRenderSystem::prepareShadowMapResources() {
    for (const auto &[id, components]: m_lightComponentRegistry->container()) {
        const auto &[transform, light] = components.get();
        light->prepareShadowMapResources(*m_ResourceManager);
    }
}
