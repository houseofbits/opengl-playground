#include "ShadowMapRenderSystem.h"

ShadowMapRenderSystem::ShadowMapRenderSystem() : EntitySystem(),
                                                 m_ResourceManager(nullptr),
                                                 m_LightsBuffer(),
                                                 m_ShaderProgram() {
    m_meshComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, StaticMeshComponent>();
    m_lightComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, LightComponent>();
    m_compositeMeshComponentRegistry = useEntityUniqueComponentRegistry<MeshComponent>();
}

void ShadowMapRenderSystem::initialize(ResourceManager &resourceManager, EventManager&) {
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
                       static_cast<int>(light->m_ShadowMaps[i]->get().m_textureRenderTarget.width),
                       static_cast<int>(light->m_ShadowMaps[i]->get().m_textureRenderTarget.height));
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
        if (mesh->m_Material.isReady() && mesh->m_Material().m_materialConfiguration.doesCastShadows) {
            m_ShaderProgram().setUniform("modelMatrix", transform->getModelMatrix());
            mesh->m_Mesh().render();
        }
    }
    for (const auto &[id, mesh]: m_compositeMeshComponentRegistry->container()) {
        if (mesh->m_Mesh().isReady()) {
            mesh->m_Mesh().render(m_ShaderProgram());
        }
    }
}

void ShadowMapRenderSystem::prepareShadowMapResources() const {
    for (const auto &[id, components]: m_lightComponentRegistry->container()) {
        const auto &[transform, light] = components.get();
        light->prepareShadowMapResources(*m_ResourceManager);
    }
}
