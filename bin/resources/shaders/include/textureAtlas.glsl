#define NUM_ATLAS_REGIONS ${NUM_ATLAS_REGIONS}

layout (std140, binding = 2) uniform AtlasRegionMappingBlock {
    vec4 atlasRegionMapping[NUM_ATLAS_REGIONS];
};

uniform sampler2D shadowDepthAtlas;
uniform sampler2D diffuseAtlas;
uniform sampler2D normalsAtlas;
uniform sampler2D effectsAtlas;

vec2 calculateAtlasUV(uint index, vec2 uv)
{   
    vec4 atlasRect = atlasRegionMapping[index];

    uv.x = (fract(uv.x) * atlasRect.x) + atlasRect.y;
    uv.y = (fract(uv.y) * atlasRect.x) + atlasRect.z;
   
    return uv;
}

bool isProjCoordsClipping(vec2 uv)
{
    if (uv.x < 0
        || uv.x > 1.0
        || uv.y < 0
        || uv.y > 1.0) {

        return false;
    }

    return true;
}

vec2 calculateAtlasUVBleed(uint index, vec2 uv)
{
    vec2 size = 1.0 / textureSize(diffuseAtlas, 0);

    vec4 atlasRect = atlasRegionMapping[index];
    atlasRect.x = atlasRect.x * (1.0 - size.x * 10.0);
    atlasRect.y = (size.x * 0.5) + atlasRect.y;
    atlasRect.z = (size.x * 0.5) + atlasRect.z;

    uv.x = (fract(uv.x) * atlasRect.x) + atlasRect.y;
    uv.y = (fract(uv.y) * atlasRect.x) + atlasRect.z;

    return uv;
}
