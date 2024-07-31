#include "ShadowMapRenderSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/StaticMeshComponent.h"

ShadowMapRenderSystem::ShadowMapRenderSystem() : EntitySystem(),
                                                 m_ResourceManager(nullptr),
                                                 m_LightsBuffer(),
                                                 m_ShaderProgram() {
    usesComponent<StaticMeshComponent>();
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
}

void ShadowMapRenderSystem::initialize(ResourceManager *resourceManager) {
    m_ResourceManager = resourceManager;
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_ShaderProgram, "data/shaders/shadowMap|.vert|.frag", {"SpotLightStorageBuffer"});
}

void ShadowMapRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &ShadowMapRenderSystem::handleEditorUIEvent);
}

bool ShadowMapRenderSystem::handleEditorUIEvent(EditorUIEvent *event) {
    return true;
}

void ShadowMapRenderSystem::process() {

    prepareShadowMapResources();

    m_ShaderProgram().use();
    m_LightsBuffer().bind(m_ShaderProgram());

    for (const auto &light: getComponentContainer<LightComponent>()) {
        if (!light.second->m_doesCastShadows || !light.second->m_isEnabled) {
            continue;
        }
        int i = 0;
        for (const auto &index: light.second->m_lightBufferIndices) {
            if (!light.second->m_ShadowMaps[i]->get().isReady()) {
                continue;
            }
            light.second->m_ShadowMaps[i]->get().bindRenderTarget();
            glViewport(0,
                       0,
                       (int) light.second->m_ShadowMaps[i]->get().m_textureRenderTarget.width,
                       (int) light.second->m_ShadowMaps[i]->get().m_textureRenderTarget.height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderGeometry(index);

            light.second->m_ShadowMaps[i]->get().unbindRenderTarget();
            i++;
        }
    }
}

void ShadowMapRenderSystem::renderGeometry(int lightIndex) {
    m_ShaderProgram().setUniform("lightIndex", lightIndex);
    for (const auto &mesh: getComponentContainer<StaticMeshComponent>()) {
        auto *transform = getComponent<TransformComponent>(mesh.first);
        if(mesh.second->m_Material.isReady() && mesh.second->m_Material().m_doesCastShadows) {
            m_ShaderProgram().setUniform("modelMatrix", transform->getModelMatrix());
            mesh.second->m_Mesh().render();
        }
    }
}

void ShadowMapRenderSystem::prepareShadowMapResources() {
    for (const auto &lightIterator: getComponentContainer<LightComponent>()) {
        lightIterator.second->prepareShadowMapResources(*m_ResourceManager);
    }
}
