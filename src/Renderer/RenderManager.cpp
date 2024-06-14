#include "RenderManager.h"
#include "../Include.h"

glm::vec3 cubeMapViewDirection[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
};
glm::vec3 cubeMapUpDirection[6] = {
        {0, -1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
        {0, -1, 0},
        {0, -1, 0},
};

RenderManager::RenderManager(Window *w) : window(w),
                                          atlasManager(),
                                          shadowMapRenderer(this),
                                          staticGeometryRenderer(this),
                                          skyBoxRenderer(this),
                                          debugInformationRenderer(this),
                                          environmentProbeRenderer(this),
                                          isLightConesVisible(false) {

    ShaderSourceLoader::registerGlobal("LIGHTS_UNIFORM_BINDING_INDEX", LIGHTS_UNIFORM_BINDING_INDEX);
}

void RenderManager::init() {
    atlasManager.init(ATLAS_REGION_MAPPING_UNIFORM_BINDING_INDEX);
    lightsUniformBuffer.create(128, LIGHTS_UNIFORM_BINDING_INDEX);
    shadowMapRenderer.init(LIGHT_INDEXES_UNIFORM_BINDING_INDEX);
    staticGeometryRenderer.init();
    skyBoxRenderer.init();
    debugInformationRenderer.init();
    environmentProbeUniformBuffer.create(32, ENV_PROBES_UNIFORM_BINDING_INDEX);
    environmentProbeRenderer.init(32);

    debugRendererColor.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageColor.frag");
}

void RenderManager::preRender(Scene &scene) {
    lightsUniformBuffer.update(scene, atlasManager);
    shadowMapRenderer.render(scene);
    environmentProbeRenderer.render(scene);
    environmentProbeUniformBuffer.update(scene);
}

void RenderManager::render(Scene &scene) {
    lightsUniformBuffer.update(scene, atlasManager);
    shadowMapRenderer.render(scene);

    glViewport(0, 0, window->viewportWidth, window->viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//    glDepthMask(GL_TRUE);
//    glDepthFunc(GL_LESS);
//
//    skyBoxRenderer.render(scene, scene.camera);
//    staticGeometryRenderer.renderDepthPrePass(scene, scene.camera);
//
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//    glDepthFunc(GL_LEQUAL);

    skyBoxRenderer.render(scene, scene.camera);
    staticGeometryRenderer.render(scene, scene.camera);

    if (isLightConesVisible) {
        debugInformationRenderer.renderLightCones(scene, scene.camera);
    }
}

void RenderManager::renderDebug(int mode) {
    if (mode == 1) {
        shadowMapRenderer.renderShadowAtlas();
    }
    if (mode == 2) {
        debugRendererColor.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_DIFFUSE);
        debugRendererColor.draw();
    }
    if (mode == 3) {
        debugRendererColor.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_EFFECTS);
        debugRendererColor.draw();
    }
    if (mode == 4) {
        debugRendererColor.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_NORMALS);
        debugRendererColor.draw();
    }
}
