#include "RendererSystem.h"
#include "../../../CoreV2/Events/WindowEvent.h"
#include <GL/glew.h>

RendererSystem::RendererSystem() : EntitySystem(),
                                   m_colorPassRenderer(this),
                                   m_frame(),
                                   m_ShaderProgram(),
                                   m_viewportWidth(1024),
                                   m_viewportHeight(768) {
    usesComponent<StaticMeshComponent>();
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void RendererSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &RendererSystem::handleWindowEvent);
}

bool RendererSystem::handleWindowEvent(WindowEvent *const event) {
    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        m_viewportWidth = event->window->viewportWidth;
        m_viewportHeight = event->window->viewportHeight;
    }

    return true;
}

void RendererSystem::initialize(ResourceManager* resourceManager) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    m_frame.initialize();
    m_colorPassRenderer.initialize(resourceManager);

    resourceManager->request(m_ShaderProgram, "data/shaders/lighting|.vert|.frag|.geom");
}

void RendererSystem::process() {

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* camera = findActiveCamera();
    assert(camera != nullptr);

//    m_ShaderProgram().use();
//    camera->bind(m_ShaderProgram());

    updateFrameData();
    m_colorPassRenderer.beginRender(*camera);
    m_colorPassRenderer.render(m_frame);
}

void RendererSystem::updateFrameData() {
    unsigned int i = 0;
    m_frame.clear();
    for (const auto &mesh: getComponentContainer<StaticMeshComponent>()) {
        m_frame.add(
                i,
                mesh.second,
                findTransform(mesh.second->m_EntityId));

        i++;
    }

    m_frame.m_LightBuffer.reset();
    m_frame.m_ProjectorBuffer.reset();
    for(const auto& light: getComponentContainer<LightComponent>()) {
        TransformComponent* transform = findTransform(light.second->m_EntityId);
        if (transform == nullptr) {
            continue;
        }

        int index = -1;
        if (light.second->m_Projection.isValid()) {
            index = m_frame.m_ProjectorBuffer.getOrAppend(light.second->m_Projection().m_handleId);
        }

        m_frame.m_LightBuffer.appendLight(*transform, *light.second, index);
    }
    m_frame.m_LightBuffer.updateAll();

    m_frame.m_EnvironmentProbeBuffer.reset();
    for(const auto& probe: getComponentContainer<EnvironmentProbeComponent>()) {
        TransformComponent* transform = findTransform(probe.second->m_EntityId);
        if (transform == nullptr) {
            continue;
        }
        m_frame.m_EnvironmentProbeBuffer.appendProbe(*transform, *probe.second);
    }
    m_frame.m_EnvironmentProbeBuffer.updateAll();
}

TransformComponent *RendererSystem::findTransform(Identity &entityId) {
    return getComponent<TransformComponent>(entityId.id());
}

Camera *RendererSystem::findActiveCamera() {
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr) {
        return &c->m_Camera;
    }

    return nullptr;
}
