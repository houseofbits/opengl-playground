#include "../Include.h"
#include "RenderManager.h"


RenderManager::RenderManager(Window *w) : window(w),atlasManager(), shadowMapRenderer(this), staticGeometryRenderer(this)
{
}

void RenderManager::init()
{
    atlasManager.init();
    lightsUniformBuffer.create(100);
    shadowMapRenderer.init();
    staticGeometryRenderer.init();

    debugRendererColor.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageColor.frag");
}

void RenderManager::render(Scene& scene)
{
    lightsUniformBuffer.update(scene, atlasManager);
    shadowMapRenderer.render(scene);
    staticGeometryRenderer.render(scene);
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
