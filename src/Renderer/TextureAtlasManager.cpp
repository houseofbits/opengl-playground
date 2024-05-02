#include "TextureAtlasManager.h"
#include "../../libs/tinygltf/stb_image.h"
#include <assert.h>
#include "TextureLoader.h"
#include "../Helper/ShaderSourceLoader.h"

TextureAtlasManager::TextureAtlasManager() : atlases()
{
    quadTree.create(6);

    ShaderSourceLoader::registerGlobal("NUM_ATLAS_REGIONS", quadTree.getNumNodes());
}

TextureAtlasManager::~TextureAtlasManager()
{
}

void TextureAtlasManager::init()
{
    // TextureAtlas &shadowDepthAtlas = atlases[ATLAS_SHADOW_DEPTH];
    // shadowDepthAtlas
    //     .setTextureBinding(0)
    //     .createAsRenderTarget(2048, 2048, Texture::TYPE_DEPTH);

    TextureAtlas &diffuseAtlas = atlases[ATLAS_DIFFUSE];
    diffuseAtlas.create(2048, 2048, Texture::TYPE_RGBA);

    TextureAtlas &effectsAtlas = atlases[ATLAS_EFFECTS];
    effectsAtlas.create(2048, 2048, Texture::TYPE_RGBA);

    initAtlasRegionsMapping();
}

int TextureAtlasManager::loadTextureIntoAtlas(std::string textureFileName, AtlasType atlasType)
{
    TextureAtlas &atlas = atlases[atlasType];

    unsigned char *data = nullptr;
    int width, height;

    if (TextureLoader::loadData(textureFileName, &width, &height, data))
    {
        int textureSize = std::max(width, height);
        int qtSize = atlas.getSize() / textureSize;

        assert(qtSize > 0);
        assert(qtSize <= 32);

        int nodeIndex = atlas.qtOccupancy.findFreeNodeOfSize(quadTree, qtSize);
        if (nodeIndex > 0)
        {
            const QuadTreeNode &node = quadTree.getNode(nodeIndex);

            int nodeSize = atlas.getSize() / node.size;

            assert(nodeSize >= textureSize);

            int left = node.left * nodeSize;
            int top = node.top * nodeSize;

            assert(nodeSize + left <= atlas.getSize());
            assert(nodeSize + top <= atlas.getSize());

            atlas.texture->applyImage(left, top, width, height, data);

            atlas.qtOccupancy.setOccupied(quadTree, nodeIndex, true);

            return nodeIndex;
        }
    }

    return -1;
}

unsigned int &TextureAtlasManager::getTextureId(AtlasType atlas)
{
    return atlases[atlas].texture->textureId;
}

void TextureAtlasManager::initAtlasRegionsMapping()
{
    glm::vec4 *data = new glm::vec4[quadTree.getNumNodes()];
    for (unsigned int i = 0; i < quadTree.getNumNodes(); i++)
    {
        const QuadTreeNode &node = quadTree.getNode(i);
        float nodeSize = 1.0f / node.size;
        data[i] = {nodeSize, node.left * nodeSize, node.top * nodeSize, 0.0f};
    }

    atlasRegionsMapping.create(quadTree.getNumNodes(), UniformBuffer<glm::vec4>::STATIC);
    atlasRegionsMapping.setData(data, 0, quadTree.getNumNodes());

    delete[] data;
}

void TextureAtlasManager::bindAll(Shader &shader)
{

    glActiveTexture(GL_TEXTURE1);
    atlases[ATLAS_DIFFUSE].texture->bind();
    shader.setUniform("diffuseAtlas", 1);

    glActiveTexture(GL_TEXTURE2);
    atlases[ATLAS_EFFECTS].texture->bind();
    shader.setUniform("effectsAtlas", 2);

    atlasRegionsMapping.bind();
    shader.setUniform("atlasRegionMapping", atlasRegionsMapping.getBufferId());
}