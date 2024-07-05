#include "MainRenderSystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>

MainRenderSystem::MainRenderSystem() : EntitySystem(), m_ShaderProgram(),
                                       m_viewportWidth(1024),
                                       m_viewportHeight(768) {
    usesComponent<StaticMeshComponent>();
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void MainRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &MainRenderSystem::handleWindowEvent);
}

bool MainRenderSystem::handleWindowEvent(WindowEvent *const event) {
    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        m_viewportWidth = event->window->viewportWidth;
        m_viewportHeight = event->window->viewportHeight;
    }

    return true;
}

void MainRenderSystem::initialize(ResourceManager *resourceManager) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    resourceManager->request(m_ShaderProgram, "data/shaders/lighting|.vert|.frag|.geom");
}

void MainRenderSystem::process() {

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera *camera = findActiveCamera();
    assert(camera != nullptr);

    m_ShaderProgram().use();
    camera->bind(m_ShaderProgram());

    //    updateFrameData();
    //    m_colorPassRenderer.beginRender(*camera);
    //    m_colorPassRenderer.render(m_frame);
}

TransformComponent *MainRenderSystem::findTransform(Identity &entityId) {
    return getComponent<TransformComponent>(entityId.id());
}

Camera *MainRenderSystem::findActiveCamera() {
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr) {
        return &c->m_Camera;
    }

    return nullptr;
}
