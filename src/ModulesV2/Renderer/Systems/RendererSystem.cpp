#include "RendererSystem.h"
#include <GL/glew.h>

RendererSystem::RendererSystem() : m_colorPassRenderer(this) {
}

void RendererSystem::initialize() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    m_colorPassRenderer.initialize();
}

void RendererSystem::process() {


    glViewport(0, 0, 1024, 768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //lights = getVisibleLights(camera);

    //for each light
    //  meshes = getVisibleMeshes(light);
    //  FrameDescriptor shadowPass = {meshes, camera:light}.
    //  m_shadowPassRenderer.render(shadowPass);

    //meshes = getVisibleMeshes(camera);
    //FrameDescriptor colorPass = {meshes, lights, probes, camera, materials, etc}.
    //m_colorPassRenderer.render(colorPass);

    assert(!m_cameraComponents.empty());

    createFrameData();
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
}

void RendererSystem::unregisterComponent(Component *comp) {
}

void RendererSystem::createFrameData() {
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
