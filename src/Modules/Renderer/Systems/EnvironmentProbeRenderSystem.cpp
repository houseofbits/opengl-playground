#include "EnvironmentProbeRenderSystem.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>

glm::vec3 EnvironmentProbeRenderSystem::m_cubeMapViewDirection[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
};
glm::vec3 EnvironmentProbeRenderSystem::m_cubeMapUpDirection[6] = {
        {0, -1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
        {0, -1, 0},
        {0, -1, 0},
};

EnvironmentProbeRenderSystem::EnvironmentProbeRenderSystem() : EntitySystem(),
                                                               m_ShaderProgram(),
                                                               m_LightsBuffer(),
                                                               m_cubeMapArray(),
                                                               m_Camera(),
                                                               m_isRenderEnabled(true) {
    usesComponent<StaticMeshComponent>();
    usesComponent<TransformComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void EnvironmentProbeRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &EnvironmentProbeRenderSystem::handleEditorUIEvent);
}

void EnvironmentProbeRenderSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_cubeMapArray, "EnvironmentProbesCubeMapArray");

    resourceManager->request(m_ShaderProgram,
                             "data/shaders/|lighting.vert|lighting.geom|lightingEnvProbe.frag",
                             {"SpotLightStorageBuffer", "EnvironmentProbeStorageBuffer"});

    m_Camera.setFieldOfView(90.0);
}

void EnvironmentProbeRenderSystem::process() {
    if (!m_isRenderEnabled || !m_cubeMapArray().isReady()) {
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glViewport(0, 0, m_cubeMapArray().TEXTURE_SIZE, m_cubeMapArray().TEXTURE_SIZE);

    bindGeometry();
    m_cubeMapArray().bindRenderTarget();
    int layer = 0;
    for (const auto &probe: getComponentContainer<EnvironmentProbeComponent>()) {
        auto *probeTransform = getComponent<TransformComponent>(probe.first);
        m_Camera.setPosition(probeTransform->getTranslation());
        probe.second->m_cubeMapLayerIndex = layer;
        //
        //        std::cout << layer << " " << probeTransform->getTranslation().x
        //                  << ", " << probeTransform->getTranslation().y
        //                  << ", " << probeTransform->getTranslation().z
        //                  << std::endl;

        glm::vec3 size = probeTransform->getScale();

        for (int i = 0; i < 6; ++i) {
            int axis = std::floor(i / 2);

            int face = layer * 6 + i;
            m_cubeMapArray().selectCubeFace(face);

            m_Camera.setZFar(size[axis] / 2);
            m_Camera.setView(m_cubeMapViewDirection[i], m_cubeMapUpDirection[i]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderGeometry();
        }

        layer++;
    }
    m_cubeMapArray().unbindRenderTarget();
    m_cubeMapArray().generateMipMaps();
    m_isRenderEnabled = false;
}

void EnvironmentProbeRenderSystem::bindGeometry() {
    m_ShaderProgram().use();
    m_LightsBuffer().bind(m_ShaderProgram());
}

void EnvironmentProbeRenderSystem::renderGeometry() {
    m_Camera.bind(m_ShaderProgram());

    for (const auto &mesh: getComponentContainer<StaticMeshComponent>()) {
        auto *transform = getComponent<TransformComponent>(mesh.first);

        assert(transform != nullptr);

        m_ShaderProgram().setUniform("modelMatrix", transform->getModelMatrix());
        mesh.second->m_Material().bind(m_ShaderProgram());
        mesh.second->m_Mesh().render();
    }
}

bool EnvironmentProbeRenderSystem::handleEditorUIEvent(EditorUIEvent *event) {
    if (event->m_Type == EditorUIEvent::TRIGGER_PROBE_RENDER) {
        m_isRenderEnabled = true;
    }

    return true;
}

float EnvironmentProbeRenderSystem::calculateZFar(glm::vec3 position, glm::vec3 direction, glm::vec3 min, glm::vec3 max) {
    glm::vec3 planeIntersect1 = (max - position) / direction;
    glm::vec3 planeIntersect2 = (min - position) / direction;

    glm::vec3 furthestPlane = glm::max(planeIntersect1, planeIntersect2);

    return std::min(std::min(furthestPlane.x, furthestPlane.y), furthestPlane.z);
}
