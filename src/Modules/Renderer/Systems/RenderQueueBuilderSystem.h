#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/Cube.h"

class ShaderResource;
class RenderQueueResource;
class MaterialResource;
class TransformComponent;
class MeshComponent;

class RenderQueueBuilderSystem : public EntitySystem {
public:
    RenderQueueBuilderSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    EntityRelatedComponentRegistry<TransformComponent, MeshComponent> *m_compositeMeshComponentRegistry;
    ResourceHandle<MaterialResource> m_defaultMaterial;
    ResourceHandle<ShaderResource> m_lightingShader;
    ResourceHandle<ShaderResource> m_skyboxShader;
    ResourceHandle<RenderQueueResource> m_mainRenderQueue;
    Cube m_box;
};