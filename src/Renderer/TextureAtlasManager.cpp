#include "TextureAtlasManager.h"
#include "../../libs/tinygltf/stb_image.h"
#include <assert.h>
#include "TextureLoader.h"
#include "../Helper/ShaderSourceLoader.h"

TextureAtlasManager::TextureAtlasManager() : atlases()
{
    quadTree.create(quadTreeDepth);

    ShaderSourceLoader::registerGlobal("NUM_ATLAS_REGIONS", quadTree.getNumNodes());
}

TextureAtlasManager::~TextureAtlasManager()
{
}

void TextureAtlasManager::init()
{
    TextureAtlas &shadowDepthAtlas = atlases[ATLAS_SHADOW_DEPTH];
    shadowDepthAtlas.createAsRenderTarget(2048, 2048, Texture::TYPE_DEPTH);

    TextureAtlas &diffuseAtlas = atlases[ATLAS_DIFFUSE];
    diffuseAtlas.create(2048, 2048, Texture::TYPE_RGBA);

    TextureAtlas &effectsAtlas = atlases[ATLAS_EFFECTS];
    effectsAtlas.create(2048, 2048, Texture::TYPE_RGBA);

    initAtlasRegionsMapping();
}

int TextureAtlasManager::loadTextureIntoAtlas(std::string textureFileName, AtlasType atlasType)
{
    TextureAtlas &atlas = getAtlas(atlasType);

    unsigned char *data = nullptr;
    int width, height;

    if (TextureLoader::loadData(textureFileName, &width, &height, data))
    {
        int textureSize = std::max(width, height);

        int nodeIndex = occupyRegion(atlasType, textureSize);
        if (nodeIndex > 0)
        {
            glm::uvec4 rect = getRegionRect(atlasType, nodeIndex);

            atlas.texture->applyImage(rect.x, rect.y, width, height, data);
        }

        return nodeIndex;
    }

    return -1;
}

int TextureAtlasManager::occupyRegion(AtlasType atlasType, int size)
{
    TextureAtlas &atlas = getAtlas(atlasType);

    int qtSize = atlas.getSize() / size;

    assert(qtSize > 0);
    assert(qtSize <= 32);

    int nodeIndex = atlas.qtOccupancy.findFreeNodeOfSize(quadTree, qtSize);
    if (nodeIndex > 0)
    {
        atlas.qtOccupancy.setOccupied(quadTree, nodeIndex, true);

        return nodeIndex;
    }

    return -1;
}

unsigned int &TextureAtlasManager::getTextureId(AtlasType type)
{
    return atlases[type].texture->textureId;
}

TextureAtlas &TextureAtlasManager::getAtlas(AtlasType type)
{
    return atlases[type];
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
    glActiveTexture(GL_TEXTURE0);
    atlases[ATLAS_SHADOW_DEPTH].bindTexture();
    shader.setUniform("diffuseAtlas", 0);

    glActiveTexture(GL_TEXTURE1);
    atlases[ATLAS_DIFFUSE].bindTexture();
    shader.setUniform("diffuseAtlas", 1);

    glActiveTexture(GL_TEXTURE2);
    atlases[ATLAS_EFFECTS].bindTexture();
    shader.setUniform("effectsAtlas", 2);

    atlasRegionsMapping.bind();
    shader.setUniform("atlasRegionMapping", atlasRegionsMapping.getBufferId());
}

glm::uvec4 TextureAtlasManager::getRegionRect(AtlasType type, unsigned int index)
{
    const QuadTreeNode &node = quadTree.getNode(index);

    int size = getAtlas(type).getSize() / node.size;

    return glm::uvec4(
        node.left * size,
        node.top * size,
        size,
        size);
}