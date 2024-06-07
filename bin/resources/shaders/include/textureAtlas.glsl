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
    vec4 atlasRect = atlasRegionMapping[index];
    uv = fract(uv) * atlasRect.x + atlasRect.yz;

    return uv;
}

vec2 calculateAtlasUvLod(uint index, vec2 uv, int lod)
{
    vec4 atlasRect = atlasRegionMapping[index];

    float texelSize = 1.0 / textureSize(diffuseAtlas, lod).x;

    atlasRect.x = atlasRect.x * (1.0 - texelSize / atlasRect.x);
    atlasRect.y = atlasRect.y + (texelSize * 0.5);
    atlasRect.z = atlasRect.z + (texelSize * 0.5);

    uv = fract(uv) * atlasRect.x + atlasRect.yz;

    return uv;
}

int calculateLodLevel(vec2 uv)
{
    vec2  dx_vtc        = dFdx(uv);
    vec2  dy_vtc        = dFdy(uv);

    float len1 = length(dx_vtc);
    float len2 = length(dy_vtc);

    float delta_max_sqr = max(len1, len2);
    float lod = (log2(delta_max_sqr * 800.0));
    int mipmapLevel = int(lod);

    if (mipmapLevel <= 0) {
        return 0;
    } else if (mipmapLevel >= 4) {
        return 4;
    }

    return mipmapLevel;
}

vec3 sampleAtlasFragmentLod(in sampler2D atlas, uint index, vec2 uv)
{
    vec4 atlasRect = atlasRegionMapping[index];
    vec2  dx_vtc        = dFdx(uv);
    vec2  dy_vtc        = dFdy(uv);

    float len1 = length(dx_vtc);// * atlasRect.x;
    float len2 = length(dy_vtc);// * atlasRect.x;

    float delta_max_sqr = max(len1, len2);
    float lod = (log2(delta_max_sqr * 800));
    int mipmapLevel = int(lod);

    if (mipmapLevel <= 0) {
        mipmapLevel = 0;
    } else if (mipmapLevel >= 6) {
        mipmapLevel = 6;
    }

    lod = min(max(lod, 0), 6);
    float f = fract(lod);
    int a = int(floor(lod));
    int b = int(ceil(lod));

    vec2 uv1 = calculateAtlasUvLod(index, uv, a);
    vec2 uv2 = calculateAtlasUvLod(index, uv, b);

    return mix(
        textureLod(atlas, uv1, a).xyz,
        textureLod(atlas, uv2, b).xyz,
        f
    );

//    return vec3(f);
    return vec3(float(mipmapLevel) / 6.0);

    //    int lod = calculateLodLevel(uv);

//    uv = calculateAtlasUvLod(index, uv, lod);

//    return textureLod(atlas, uv, lod).xyz;
}

vec3 sampleDiffuseAtlasFragment(uint index, vec2 uv, vec3 diffuseColor)
{
    if (index > 0) {
        return sampleAtlasFragmentLod(diffuseAtlas, index, uv).xyz * diffuseColor;
    }
    return diffuseColor;
}

vec3 sampleNormalsAtlasFragment(uint index, vec2 uv, vec3 faceNormal, mat3 invTBN)
{
    if (index > 0) {
        vec3 n = sampleAtlasFragmentLod(normalsAtlas, index, uv).xyz;
        n = gsInvTBN * normalize(n * 2.0 - 1.0);

        return normalize(n);
    }
    return normalize(faceNormal);
}
