#define NUM_SHADOW_ATLAS_REGIONS ${NUM_SHADOW_ATLAS_REGIONS}

uniform sampler2D shadowDepthAtlas;

layout (std140, binding = 2) uniform ShadowAtlasRegionsBlock {
    vec4 shadowAtlasRegions[NUM_SHADOW_ATLAS_REGIONS];
};

vec3 getProjectedCoords2(LightStructure light, vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    vec4 pos = shadowAtlasRegions[light.shadowAtlasIndex];
    projCoords.x = (projCoords.x * pos.z) + pos.x;
    projCoords.y = (projCoords.y * pos.w) + pos.y;

    return projCoords;
}

vec3 getProjectedCoords(LightStructure light, vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    projCoords.xy = (projCoords.xy * light.shadowAtlasSize) + light.shadowAtlasPos;

    return projCoords;
}

bool projCoordsClip(LightStructure light, vec3 projCoords)
{
    if (projCoords.z > 1.0 
        || projCoords.x < light.shadowAtlasPos.x 
        || projCoords.x > (light.shadowAtlasPos.x + light.shadowAtlasSize.x) 
        || projCoords.y < light.shadowAtlasPos.y 
        || projCoords.y > (light.shadowAtlasPos.y + light.shadowAtlasSize.y)) {

        return false;
    }

    return true;
}

float sampleShadow(vec3 projCoords, float bias)
{
    float depth = texture(shadowDepthAtlas, projCoords.xy).r; 
    return projCoords.z - bias > depth ? 0.0 : 1.0;    
}

float sampleShadow(vec3 projCoords, float bias, vec2 texelSize, int x, int y)
{
    projCoords.xy = projCoords.xy + vec2(x, y) * texelSize;
    return sampleShadow(projCoords, bias);
}

float pcfShadowCalculation(vec3 projCoords, float ndotl)
{
    float currentDepth = projCoords.z;

    float bias = 0.0000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    vec2 texelSize = 1.0 / textureSize(shadowDepthAtlas, 0);
    float shadow = 0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            shadow += sampleShadow(projCoords, bias, texelSize, x,y);      
        }    
    }
    shadow /= 9.0;

    return shadow;
}