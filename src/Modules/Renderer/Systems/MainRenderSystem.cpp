#include "MainRenderSystem.h"
#include "../Components/SkyComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>
#include "../../Application/Events/SystemEvent.h"
#include "../Resources/DeferredRenderTargetResource.h"
#include "../../../Renderer/Queue/RenderQueueBuilder.h"
#include "../Resources/RenderQueueResource.h"

MainRenderSystem::MainRenderSystem() : EntitySystem(),
                                       m_isEnabled(true),
                                       m_shaderType(SHADER_SHADED),
                                       m_viewportWidth(1024),
                                       m_viewportHeight(768),
                                       m_ShaderPrograms(),
                                       m_defaultMaterial(),
                                       m_LightsBuffer(),
                                       m_MaterialsBuffer(),
                                       m_ProbesBuffer(),
                                       m_brdfLUTTexture(),
                                       m_activeCameraHelper() {
    m_meshComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, StaticMeshComponent>();
    m_skyComponentRegistry = useEntityUniqueComponentRegistry<SkyComponent>();
    m_compositeMeshComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, MeshComponent>();
}

void MainRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleEditorUIEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleCameraActivationEvent);
}

void MainRenderSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::WINDOW_RESIZED || event.eventType == SystemEvent::WINDOW_CREATED) {
        m_viewportWidth = event.window->viewportWidth;
        m_viewportHeight = event.window->viewportHeight;
    }
}

void MainRenderSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    resourceManager.request(m_ShaderPrograms[SHADER_SHADED],
                            // "data/shaders/|lighting.vert|lighting-shadow-test.frag",
                            "data/shaders/lighting|.vert|.frag",
                            // "data/shaders/|lighting.vert|lightingEnvProbe.frag",
                            {
                                "SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                                "EnvironmentProbesCubeMapArray"
                            });

    resourceManager.request(m_ShaderPrograms[SHADER_PROBES],
                            "data/shaders/|lighting.vert|lightingProbeEdit.frag",
                            {
                                "SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                                "EnvironmentProbesCubeMapArray"
                            });

    resourceManager.request(m_ShaderPrograms[SHADER_REFLECTION],
                            "data/shaders/|lighting.vert|lightingReflection.frag",
                            {
                                "SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                                "EnvironmentProbesCubeMapArray"
                            });

    resourceManager.request(m_ShaderPrograms[SHADER_SKY], "data/shaders/skybox.|vert|frag",
                            {
                                "SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                                "EnvironmentProbesCubeMapArray"
                            }
    );

    resourceManager.request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager.request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
    resourceManager.request(m_ProbesCubeMapArray, "EnvironmentProbesCubeMapArray");
    resourceManager.request(m_MaterialsBuffer, "MaterialsStorageBuffer");

    resourceManager.requestWith(m_defaultMaterial, "defaultMaterial",
                                [&](MaterialResource &resource) {
                                    resource.fetchDefault(resourceManager);
                                });

    resourceManager.request(m_deferredRenderTarget, "deferredRenderTarget");
    resourceManager.request(m_brdfLUTTexture, "data/textures/ibl_brdf_lut.png");

    resourceManager.request(m_lightingShader, "data/shaders/definitions/lighting.json");
    resourceManager.request(m_mainRenderQueue, "mainRenderQueue");
}

void MainRenderSystem::process(EventManager &eventManager) {
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // if (!m_deferredRenderTarget().isReady()) {
    //     return;
    // }

    // m_deferredRenderTarget().bindRenderTarget();
    // glViewport(0,
    //            0,
    //            m_viewportWidth,
    //            m_viewportHeight);
    // glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
    if (camera == nullptr) {
        return;
    }

    const auto &sky = m_skyComponentRegistry->container().begin();
    const bool doesSkyComponentExist = sky != m_skyComponentRegistry->container().end();
    if (doesSkyComponentExist && sky->second->m_cubeMap().isReady()) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        m_ShaderPrograms[SHADER_SKY]().use();
        m_LightsBuffer().bind(m_ShaderPrograms[SHADER_SKY].get());
        m_ShaderPrograms[SHADER_SKY]().setUniform("environmentSampler", sky->second->m_cubeMap().m_handleId);
        camera->bind(m_ShaderPrograms[SHADER_SKY]());
        sky->second->m_box.draw();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (!m_isEnabled || !m_lightingShader.isReady()) {
        return;
    }

    m_mainRenderQueue().clear();

    for (const auto &[id, components]: m_compositeMeshComponentRegistry->container()) {
        if (const auto &[transform, mesh] = components.get(); mesh->m_Mesh().isReady()) {
            mesh->addToRenderQueue(m_mainRenderQueue.get(), m_lightingShader().getShader(),
                                   transform->getWorldTransform(), m_defaultMaterial.get());
        }
    }

    m_mainRenderQueue().render(*camera);
}

void MainRenderSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    m_isEnabled = true;
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_SHADED) {
        m_shaderType = SHADER_SHADED;
    }
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_PROBES) {
        m_shaderType = SHADER_PROBES;
    }
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_REFLECTIONS) {
        m_shaderType = SHADER_REFLECTION;
    }
    if (event.m_Type == EditorUIEvent::TOGGLE_RENDER_PHYSICS) {
        m_isEnabled = false;
    }
}
