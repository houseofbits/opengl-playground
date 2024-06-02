#pragma once
#include "../../Helper/Console.h"
#include "../Shader/UniformBuffer.h"
#include "TextureAtlas.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <map>
#include <math.h>
#include <string>

class TextureAtlasNode {
public:
    int parentIndex;
    int childIndices[4];
    unsigned int left;
    unsigned int top;
    unsigned int size;
};

class TextureAtlasManager {
public:
    static const unsigned int quadTreeDepth = 6;

    enum AtlasType {
        ATLAS_SHADOW_DEPTH,
        ATLAS_DIFFUSE,
        ATLAS_NORMALS,
        ATLAS_EFFECTS,
    };

    TextureAtlasManager();
    ~TextureAtlasManager();

    void init(unsigned int uniformBindingIndex);
    int loadTextureIntoAtlas(std::string textureFileName, AtlasType atlas);
    unsigned int &getTextureId(AtlasType atlas);
    TextureAtlas &getAtlas(AtlasType type);
    void bindAll(Shader &shader);
    glm::uvec4 getRegionRect(AtlasType type, unsigned int index);
    int occupyRegion(AtlasType type, int size);

private:
    QuadTree quadTree;
    UniformBuffer<glm::vec4> atlasRegionsMapping;
    std::map<AtlasType, TextureAtlas> atlases;
    std::map<std::string, std::pair<unsigned int, AtlasType>> loadedTextures;

    void initAtlasRegionsMapping(unsigned int uniformBindingIndex);
};