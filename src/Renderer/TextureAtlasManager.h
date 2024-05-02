#pragma once
#include "TextureAtlas.h"
#include "../Helper/Console.h"
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <glm/vec3.hpp>
#include "UniformBuffer.h"

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
    unsigned int &getTextureId(AtlasType atlas);
    void bindAll(Shader &shader);

private:
    QuadTree quadTree;
    UniformBuffer<glm::vec4> atlasRegionsMapping;
    std::map<AtlasType, TextureAtlas> atlases;

    void initAtlasRegionsMapping();
};