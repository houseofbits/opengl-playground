#include "EnvironmentProbeRenderer.h"
#include "../RenderManager.h"

glm::vec3 EnvironmentProbeRenderer::cubeMapViewDirection[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
};
glm::vec3 EnvironmentProbeRenderer::cubeMapUpDirection[6] = {
        {0, -1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
        {0, -1, 0},
        {0, -1, 0},
};

EnvironmentProbeRenderer::EnvironmentProbeRenderer(RenderManager *manager) : BaseRenderer(manager), cubeRenderTarget(), camera() {
}

void EnvironmentProbeRenderer::init(int maxProbes) {
    cubeRenderTarget.createArray(PROBE_RESOLUTION, PROBE_RESOLUTION, Texture::TYPE_RGBA, maxProbes);
    camera.setViewportSize(PROBE_RESOLUTION, PROBE_RESOLUTION)
            .setFieldOfView(90.0);
}

void EnvironmentProbeRenderer::render(Scene &scene) {
    cubeRenderTarget.bind();
    int layer = 0;
    for (const auto &probe: scene.probes) {
        for (int i = 0; i < 6; ++i) {
            int face = layer * 6 + i;
            glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeRenderTarget.textureId, 0, face);

            glViewport(0, 0, PROBE_RESOLUTION, PROBE_RESOLUTION);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            probe->cubeMapTextureLayer = layer;
            camera.setPosition(probe->position);
            camera.setView(cubeMapViewDirection[i], cubeMapUpDirection[i]);

            renderManager->skyBoxRenderer.render(scene, camera);
            renderManager->staticGeometryRenderer.renderSimplified(scene, camera);


        }

        std::cout << "Render probe: "
                  << layer
                  << " - "
                  << probe->position.x
                  << ","
                  << probe->position.y
                  << ","
                  << probe->position.z
                  << std::endl;
        ;

        layer++;
    }
    cubeRenderTarget.unbind();

    glGenerateTextureMipmap(cubeRenderTarget.textureId);
}
