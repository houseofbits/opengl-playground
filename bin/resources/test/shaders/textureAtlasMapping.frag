#version 450

layout (location=0) out vec4 fragColor;

out vec4 position;
in vec3 normal;
in vec2 uv;

#define NUM_ATLAS_REGIONS ${NUM_ATLAS_REGIONS}

layout (std140, binding = 2) uniform AtlasRegionMappingBlock {
    vec4 atlasRegionMapping[NUM_ATLAS_REGIONS];
};

uniform sampler2D diffuseAtlas;
uniform sampler2D effectsAtlas;
uniform uint diffuseTextureId;

vec2 calculateAtlasUV(uint index, vec2 uv)
{
    vec2 size = 1.0 / textureSize(diffuseAtlas, 0);

    vec4 atlasRect = atlasRegionMapping[index];

    atlasRect.x = atlasRect.x * (1.0 - size.x * 10.0);
    atlasRect.y = (size.x * 0.5) + atlasRect.y;
    atlasRect.z = (size.x * 0.5) + atlasRect.z;

    uv.x = (uv.x * atlasRect.x) + atlasRect.y;
    uv.y = (uv.y * atlasRect.x) + atlasRect.z;
   
    return uv;
}

void main()
{   
    vec2 uv2 = calculateAtlasUV(diffuseTextureId, uv);

    vec4 color = texture(diffuseAtlas, uv2);

    fragColor = vec4(color.rgb, 1.0);
}
