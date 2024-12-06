#include "MainRenderSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/SkyComponent.h"
#include "../Components/StaticMeshComponent.h"
#include "../../Editor/Components/EditorCameraComponent.h"
#include <GL/glew.h>

MainRenderSystem::MainRenderSystem() : EntitySystem(),
                                       m_isEnabled(true),
                                       m_shaderType(SHADER_SHADED),
                                       m_ShaderPrograms(),
                                       m_LightsBuffer(),
                                       m_ProbesBuffer(),
                                       m_viewportWidth(1024),
                                       m_viewportHeight(768),
                                       m_activeCameraId(0) {
    usesComponent<StaticMeshComponent>();
    usesComponent<TransformComponent>();
//    usesComponent<CameraComponent>();
//    usesComponent<EditorCameraComponent>();
    usesComponent<SkyComponent>();
}

void MainRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleEditorUIEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleCameraActivationEvent);
}

void MainRenderSystem::handleWindowEvent(const WindowEvent & event) {
    if (event.eventType == WindowEvent::RESIZE || event.eventType == WindowEvent::CREATE) {
        m_viewportWidth = event.window->viewportWidth;
        m_viewportHeight = event.window->viewportHeight;
    }
}

void MainRenderSystem::initialize(ResourceManager &resourceManager) {
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

    Camera *camera = findActiveCamera();
    if (camera == nullptr) {
        return;
    }

    auto sky = getComponentContainer<SkyComponent>().begin();
    if (doesComponentsExist<SkyComponent>() && sky->second->m_cubeMap().isReady()) {
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
    if (doesComponentsExist<SkyComponent>() && sky->second->m_cubeMap().isReady()) {
        m_ShaderPrograms[m_shaderType]().setUniform("environmentSampler", sky->second->m_cubeMap().m_handleId);
    }

    for (const auto &mesh: getComponentContainer<StaticMeshComponent>()) {
        if (mesh.second->m_targetRenderer != StaticMeshComponent::SOLID) {
            continue;
        }
        auto *transform = getComponent<TransformComponent>(mesh.first);
        m_ShaderPrograms[m_shaderType]().setUniform("modelMatrix", transform->getModelMatrix());
        mesh.second->m_Material().bind(m_ShaderPrograms[m_shaderType]());
        mesh.second->m_Mesh().render();
    }
}

Camera *MainRenderSystem::findActiveCamera() {
    if (!m_activeCameraId) {
        return nullptr;
    }

    auto entity = m_EntityContext->getEntity(m_activeCameraId);

    if (!entity) {
        return nullptr;
    }

    auto camera = entity->getComponent<CameraComponent>();
    if (camera != nullptr) {
        return &camera->m_Camera;
    }

    auto editorCamera = entity->getComponent<EditorCameraComponent>();
    if (editorCamera != nullptr) {
        return &editorCamera->m_Camera;
    }

    return nullptr;
}

void MainRenderSystem::handleEditorUIEvent(const EditorUIEvent & event) {
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

void MainRenderSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    m_activeCameraId = event.m_cameraComponentId;
}
