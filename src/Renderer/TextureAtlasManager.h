#pragma once
#include "TextureAtlas.h"
#include "../Helper/Console.h"
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <glm/vec3.hpp>

class TextureAtlasNode
{
public:
    int parentIndex;
    int childIndices[4];
    unsigned int left;
    unsigned int top;
    unsigned int size;
};

class TextureAtlasManager
{
public:
    enum AtlasType
    {
        ATLAS_SHADOW_DEPTH,
        ATLAS_DIFFUSE,
        ATLAS_EFFECTS,
    };

    TextureAtlasManager();
    ~TextureAtlasManager();
    void init();
    int loadTextureIntoAtlas(std::string textureFileName, AtlasType atlas);
    void printNodes(std::string indent = "", int index = 0);
    void printNodesWithRegions(int inputSize, std::string indent = "", int index = 0);

    int occupyAtlasRegion(TextureAtlas &atlas, const unsigned int &width, const unsigned int &height);

private:
    const unsigned int maxDepth = 6;
    int sizeIterator = 0;
    unsigned int numNodes;
    TextureAtlasNode *nodes;
    std::map<AtlasType, TextureAtlas> atlases;

    void generateNodes();
    void generateNodesRecursive(int parentIndex, unsigned int depth);
    glm::vec3 *createIndexedRegionArray();

    int findFirstAvailable(TextureAtlas &atlas, const unsigned int &width, const unsigned int &height, unsigned int nodeIndex);
    int areChildrenOccupied(TextureAtlas &atlas, unsigned int index);
};