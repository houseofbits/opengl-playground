#include "TextureAtlasManager.h"

TextureAtlasManager::TextureAtlasManager() : atlases()
{
    numNodes = 1;
    for (int i = 1; i < maxDepth; i++)
        numNodes += pow(4, i);

    nodes = new TextureAtlasNode[numNodes];
    generateNodes();
}

TextureAtlasManager::~TextureAtlasManager()
{
    delete[] nodes;
}

void TextureAtlasManager::init()
{
    TextureAtlas &shadowDepthAtlas = atlases[ATLAS_SHADOW_DEPTH];
    shadowDepthAtlas
        .setTextureBindingId(0)
        .setSize(2048)
        .enableAsRenderTarget(RenderTarget::TARGET_DEPTH);

    TextureAtlas &diffuseAtlas = atlases[ATLAS_DIFFUSE];
    diffuseAtlas
        .setTextureBindingId(1)
        .setSize(2048)
        .enableColorTexture();

    TextureAtlas &effectsAtlas = atlases[ATLAS_EFFECTS];
    shadowDepthAtlas
        .setTextureBindingId(2)
        .setSize(1024)
        .enableColorTexture();
}

int TextureAtlasManager::loadTextureIntoAtlas(std::string textureFileName, AtlasType atlas)
{
    unsigned int imageWidth;
    unsigned int imageHeight;

    // TODO: load image

    int index = occupyAtlasRegion(atlases[atlas], imageWidth, imageHeight);
    // if (index > 0)
    // {
    //     // atlases[atlas].writeImage(index, imageData, imageWidth, imageHeight);
    // }
    // else
    // {
    //     std::cout
    //         << FORERED
    //         << "FAILED TO ALLOCATE ATLAS REGION: "
    //         << textureFileName
    //         << RESETTEXT
    //         << std::endl;
    // }

    return index;
}

int TextureAtlasManager::occupyAtlasRegion(TextureAtlas &atlas, const unsigned int &width, const unsigned int &height)
{
    int index = findFirstAvailable(atlas, width, height, 0);
    if (index >= 0)
    {
        atlas.setOccupied(index, true);
    }

    return index;
}

int TextureAtlasManager::findFirstAvailable(TextureAtlas &atlas, const unsigned int &width, const unsigned int &height, unsigned int nodeIndex)
{
    TextureAtlasNode &node = nodes[nodeIndex];
    int nodeWidth = atlas.width / node.size;
    int nodeHeight = atlas.height / node.size;

    if (nodeWidth >= width * 2 && nodeHeight >= height * 2)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            if (nodes[nodeIndex].childIndices[i] >= 0)
            {
                int index = findFirstAvailable(atlas, width, height, nodes[nodeIndex].childIndices[i]);
                if (index >= 0)
                {
                    return index;
                }
            }
        }
    }

    return areChildrenOccupied(atlas, nodeIndex)
               ? -1
               : nodeIndex;
}

int TextureAtlasManager::areChildrenOccupied(TextureAtlas &atlas, unsigned int index)
{
    bool isOccupied = atlas.isOccupied(index);
    if (!isOccupied)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            if (nodes[index].childIndices[i] >= 0)
            {
                return areChildrenOccupied(atlas, nodes[index].childIndices[i]);
            }
        }
    }

    return isOccupied;
}

void TextureAtlasManager::generateNodes()
{
    sizeIterator = 0;

    TextureAtlasNode node;
    node.parentIndex = -1;
    node.size = 1;
    node.left = 0;
    node.top = 0;
    node.childIndices[0] = -1;
    node.childIndices[1] = -1;
    node.childIndices[2] = -1;
    node.childIndices[3] = -1;
    nodes[sizeIterator] = node;
    sizeIterator++;

    generateNodesRecursive(0, 1);
}

void TextureAtlasManager::generateNodesRecursive(int parentIndex, unsigned int depth)
{
    if (depth + 1 > maxDepth)
    {
        return;
    }

    TextureAtlasNode &parent = nodes[parentIndex];
    unsigned int i = 0;
    for (unsigned int x = 0; x < 2; x++)
    {
        for (unsigned int y = 0; y < 2; y++)
        {
            TextureAtlasNode node;
            node.parentIndex = parentIndex;
            node.size = parent.size * 2;
            node.left = (parent.left * 2) + x;
            node.top = (parent.top * 2) + y;
            node.childIndices[0] = -1;
            node.childIndices[1] = -1;
            node.childIndices[2] = -1;
            node.childIndices[3] = -1;

            unsigned int nodeIndex = sizeIterator;
            nodes[nodeIndex] = node;

            parent.childIndices[i] = nodeIndex;

            sizeIterator++;
            i++;

            generateNodesRecursive(nodeIndex, depth + 1);
        }
    }
}

glm::vec3 *TextureAtlasManager::createIndexedRegionArray()
{
    glm::vec3 *array = new glm::vec3[numNodes];
    for (unsigned int i = 0; i < numNodes; i++)
    {

        float nodeSize = 1.0 / nodes[i].size;
        float nodeLeft = nodeSize * nodes[i].left;
        float nodeTop = nodeSize * nodes[i].top;

        array[i] = glm::vec3(nodeSize, nodeLeft, nodeTop);
    }

    return array;
}

void TextureAtlasManager::printNodes(std::string indent, int index)
{
    const TextureAtlasNode &node = nodes[index];
    std::cout
        << FOREYEL
        << indent
        << index
        << ": "
        << FORECYN
        << node.size
        << " "
        << node.left
        << ","
        << node.top
        // << " "
        // << node.childIndices[0] << "," << node.childIndices[1] << "," << node.childIndices[2] << "," << node.childIndices[3]
        << std::endl;

    if (node.childIndices[0] >= 0)
    {
        printNodes(indent + " ", node.childIndices[0]);
        printNodes(indent + " ", node.childIndices[1]);
        printNodes(indent + " ", node.childIndices[2]);
        printNodes(indent + " ", node.childIndices[3]);
    }
}

void TextureAtlasManager::printNodesWithRegions(int inputSize, std::string indent, int index)
{
    const TextureAtlasNode &node = nodes[index];

    float nodeSize = inputSize / node.size;
    float nodeLeft = nodeSize * node.left;
    float nodeTop = nodeSize * node.top;

    std::cout
        << FOREYEL
        << indent
        << index
        << ": "
        << FORECYN
        << nodeSize
        << " "
        << nodeLeft
        << ","
        << nodeTop
        << std::endl;

    if (node.childIndices[0] >= 0)
    {
        printNodesWithRegions(inputSize, indent + " ", node.childIndices[0]);
        printNodesWithRegions(inputSize, indent + " ", node.childIndices[1]);
        printNodesWithRegions(inputSize, indent + " ", node.childIndices[2]);
        printNodesWithRegions(inputSize, indent + " ", node.childIndices[3]);
    }
}
