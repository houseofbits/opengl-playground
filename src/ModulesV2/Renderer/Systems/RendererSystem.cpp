#include "RendererSystem.h"
#include "../../../CoreV2/Events/WindowEvent.h"
#include <GL/glew.h>

RendererSystem::RendererSystem() : EntitySystem(),
                                   m_colorPassRenderer(this),
                                   m_frame(),
                                   m_viewportWidth(1024),
                                   m_viewportHeight(768) {
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
}

void RendererSystem::process() {

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    //lights = getVisibleLights(camera);

    //for each light
    //  meshes = getVisibleMeshes(light);
    //  FrameDescriptor shadowPass = {meshes, camera:light}.
    //  m_shadowPassRenderer.render(shadowPass);

    //meshes = getVisibleMeshes(camera);
    //FrameDescriptor colorPass = {meshes, lights, probes, camera, materials, etc}.
    //m_colorPassRenderer.render(colorPass);

    assert(!m_cameraComponents.empty());

    updateFrameData();
    m_colorPassRenderer.beginRender(m_cameraComponents.begin()->second->m_Camera);
    m_colorPassRenderer.render(m_frame);
}

void RendererSystem::registerComponent(Component *comp) {
    if (isOfType<StaticMeshComponent>(comp)) {
        m_meshComponents[comp->m_EntityId()] = dynamic_cast<StaticMeshComponent *>(comp);
    }
    if (isOfType<TransformComponent>(comp)) {
        m_transformComponents[comp->m_EntityId()] = dynamic_cast<TransformComponent *>(comp);
    }
    if (isOfType<CameraComponent>(comp)) {
        m_cameraComponents[comp->m_EntityId()] = dynamic_cast<CameraComponent *>(comp);
    }
    if (isOfType<MaterialComponent>(comp)) {
        m_materialComponents[comp->m_EntityId()] = dynamic_cast<MaterialComponent *>(comp);
    }
    if (isOfType<LightComponent>(comp)) {
        m_lightComponents[comp->m_EntityId()] = dynamic_cast<LightComponent *>(comp);
    }
}

void RendererSystem::unregisterComponent(Component *comp) {
}

void RendererSystem::updateFrameData() {
    unsigned int i = 0;
    m_frame.clear();
    for (const auto &mesh: m_meshComponents) {
        m_frame.add(
                i,
                mesh.second,
                findTransform(mesh.second->m_EntityId),
                findMaterial(mesh.second->m_EntityId));

        i++;
    }

    m_frame.m_SpotLightBuffer.reset();
    for(const auto& light: m_lightComponents) {
        TransformComponent* transform = findTransform(light.second->m_EntityId);
        if (transform == nullptr) {
            continue;
        }

        if (light.second->m_Type == LightComponent::SPOT) {
            m_frame.m_SpotLightBuffer.appendLight(*transform, *light.second);
        }
    }
    m_frame.m_SpotLightBuffer.updateAll();
}

TransformComponent *RendererSystem::findTransform(Identity &entityId) {
    auto iter = m_transformComponents.find(entityId());
    if (iter != m_transformComponents.end()) {
        return iter->second;
    }

    return nullptr;
}

MaterialComponent *RendererSystem::findMaterial(Identity &entityId) {
    auto iter = m_materialComponents.find(entityId());
    if (iter != m_materialComponents.end()) {
        return iter->second;
    }

    return nullptr;
}
