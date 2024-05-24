#include "../Include.h"
#include "RenderManager.h"


RenderManager::RenderManager(Window *w) :
                                          window(w),
                                          atlasManager(),
                                          shadowMapRenderer(this),
                                          staticGeometryRenderer(this),
                                          skyBoxRenderer(this),
                                          cubeRenderTarget(),
                                          cubeCamera()
{
}

void RenderManager::init()
{
    atlasManager.init();
    lightsUniformBuffer.create(100);
    shadowMapRenderer.init();
    staticGeometryRenderer.init();
    skyBoxRenderer.init();

    debugRendererColor.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageColor.frag");

    cubeRenderTarget.create(256, 256, Texture::TYPE_RGBA);
}

void RenderManager::render(Scene& scene)
{
    lightsUniformBuffer.update(scene, atlasManager);
    shadowMapRenderer.render(scene);

    cubeRenderTarget.bind();
//    cubeCamera.setPosition(scene.camera.getPosition());
    for (int i = 0; i < 6; ++i) {

        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeRenderTarget.textureId, 0);

        glViewport(0, 0, 256, 256);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        staticGeometryRenderer.render(scene, scene.camera);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    cubeRenderTarget.unbind();

    glViewport(0, 0, window->viewportWidth, window->viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skyBoxRenderer.render(scene);
    staticGeometryRenderer.render(scene, scene.camera);
}

void RenderManager::renderDebug(int mode)
{
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
