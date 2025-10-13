#include "RenderQueueBuilderSystem.h"
#include "../Resources/ShaderResource.h"
#include "../Resources/RenderQueueResource.h"
#include "../Resources/MaterialResource.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/MeshComponent.h"

RenderQueueBuilderSystem::RenderQueueBuilderSystem() : EntitySystem(), m_box() {
    m_compositeMeshComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, MeshComponent>();
}

void RenderQueueBuilderSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_lightingShader, "data/shaders/metadata/lighting.json");
    resourceManager.request(m_skyboxShader, "data/shaders/metadata/skybox.json");
    resourceManager.request(m_mainRenderQueue, "mainRenderQueue");
    resourceManager.requestWith(m_defaultMaterial, "defaultMaterial",
                                [&](MaterialResource &resource) {
                                    resource.fetchDefault(resourceManager);
                                });
    m_box.create(10.0);
}

void RenderQueueBuilderSystem::process(EventManager &) {
    m_mainRenderQueue().clear();

    if (m_skyboxShader.isReady()) {
        RenderQueueItem item;
        item.shader = &m_skyboxShader().getShader();
        item.vertexArray = m_box.vertexArrayObjectId;
        item.offset = 0;
        item.size = m_box.elementsArray[0]->count;
        item.modelMatrix = glm::mat4(1.0);

        m_mainRenderQueue().add(item);
    }

    if (m_lightingShader.isReady()) {
        for (const auto &[id, components]: m_compositeMeshComponentRegistry->container()) {
            if (const auto &[transform, mesh] = components.get(); mesh->m_Mesh().isReady()) {
                mesh->addToRenderQueue(m_mainRenderQueue.get(), m_lightingShader().getShader(),
                                       transform->getWorldTransform(), m_defaultMaterial.get());
            }
        }
    }
}

void RenderQueueBuilderSystem::registerEventHandlers(EventManager &eventManager) {
}
