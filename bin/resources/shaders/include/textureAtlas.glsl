#define NUM_ATLAS_REGIONS ${NUM_ATLAS_REGIONS}

layout (std140, binding = 2) uniform AtlasRegionMappingBlock {
    vec4 atlasRegionMapping[NUM_ATLAS_REGIONS];
};

uniform sampler2D shadowDepthAtlas;
uniform sampler2D diffuseAtlas;
uniform sampler2D effectsAtlas;

vec2 calculateAtlasUV(uint index, vec2 uv)
{   
    vec4 atlasRect = atlasRegionMapping[index];

    uv.x = (uv.x * atlasRect.x) + atlasRect.y;
    uv.y = (uv.y * atlasRect.x) + atlasRect.z;
   
    return uv;
}

bool isProjCoordsClipping(uint index, vec2 uv)
{
    vec4 atlasRect = atlasRegionMapping[index];

    if (uv.x < atlasRect.y 
        || uv.x > (atlasRect.x + atlasRect.y) 
        || uv.y < atlasRect.z 
        || uv.y > (atlasRect.x + atlasRect.z)) {

        return false;
    }

    return true;
}