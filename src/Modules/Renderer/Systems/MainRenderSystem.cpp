#include "MainRenderSystem.h"
#include "../Components/SkyComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>
#include "../../Application/Events/SystemEvent.h"

MainRenderSystem::MainRenderSystem() : EntitySystem(),
                                       m_isEnabled(true),
                                       m_shaderType(SHADER_SHADED),
                                       m_viewportWidth(1024),
                                       m_viewportHeight(768),
                                       m_ShaderPrograms(),
                                       m_LightsBuffer(),
                                       m_ProbesBuffer(),
                                       m_activeCameraHelper() {
    m_meshComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, StaticMeshComponent>();
    m_skyComponentRegistry = useEntityUniqueComponentRegistry<SkyComponent>();
    m_compositeMeshComponentRegistry = useEntityUniqueComponentRegistry<MeshComponent>();
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

void MainRenderSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    resourceManager.request(m_ShaderPrograms[SHADER_SHADED],
                            "data/shaders/lighting|.vert|.frag|.geom",
                            {"SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                             "EnvironmentProbesCubeMapArray"});

    resourceManager.request(m_ShaderPrograms[SHADER_PROBES],
                            "data/shaders/|lighting.vert|lighting.geom|lightingProbeEdit.frag",
                            {"SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                             "EnvironmentProbesCubeMapArray"});

    resourceManager.request(m_ShaderPrograms[SHADER_REFLECTION],
                            "data/shaders/|lighting.vert|lighting.geom|lightingReflection.frag",
                            {"SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer",
                             "EnvironmentProbesCubeMapArray"});

    resourceManager.request(m_ShaderPrograms[SHADER_SKY], "data/shaders/skybox.|vert|frag");

    resourceManager.request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager.request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
    resourceManager.request(m_ProbesCubeMapArray, "EnvironmentProbesCubeMapArray");
}

void MainRenderSystem::process(EventManager &eventManager) {

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
    if (camera == nullptr) {
        return;
    }

    const auto& sky = m_skyComponentRegistry->container().begin();
    bool doesSkyComponentExist = sky != m_skyComponentRegistry->container().end();
    if (doesSkyComponentExist && sky->second->m_cubeMap().isReady()) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        m_ShaderPrograms[SHADER_SKY]().use();
        m_ShaderPrograms[SHADER_SKY]().setUniform("environmentSampler", sky->second->m_cubeMap().m_handleId);
        camera->bind(m_ShaderPrograms[SHADER_SKY]());
        sky->second->m_box.draw();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (!m_isEnabled) {
        return;
    }

    m_ShaderPrograms[m_shaderType]().use();
    camera->bind(m_ShaderPrograms[m_shaderType]());
    m_LightsBuffer().bind(m_ShaderPrograms[m_shaderType]());
    m_ProbesBuffer().bind(m_ShaderPrograms[m_shaderType]());
    if (m_ProbesCubeMapArray().isReady()) {
        m_ShaderPrograms[m_shaderType]().setUniform("probesCubeArraySampler", m_ProbesCubeMapArray().m_handleId);
    }
    if (doesSkyComponentExist && sky->second->m_cubeMap().isReady()) {
        m_ShaderPrograms[m_shaderType]().setUniform("environmentSampler", sky->second->m_cubeMap().m_handleId);
    }

    for (const auto &[id, components]: m_meshComponentRegistry->container()) {
        const auto &[transform, mesh] = components.get();
        m_ShaderPrograms[m_shaderType]().setUniform("modelMatrix", transform->getModelMatrix());
        mesh->m_Material().bind(m_ShaderPrograms[m_shaderType]());
        mesh->m_Mesh().render();
    }

    for (const auto &component: m_compositeMeshComponentRegistry->container()) {
        component.second->m_Material().bind(m_ShaderPrograms[m_shaderType]());
        component.second->m_Mesh().render(m_ShaderPrograms[m_shaderType]());
    }
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
