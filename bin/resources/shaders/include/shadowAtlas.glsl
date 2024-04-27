#define NUM_SHADOW_ATLAS_REGIONS ${NUM_SHADOW_ATLAS_REGIONS}

uniform sampler2D shadowDepthAtlas;

layout (std140, binding = 2) uniform ShadowAtlasRegionsBlock {
    vec4 shadowAtlasRegions[NUM_SHADOW_ATLAS_REGIONS];
};

vec3 getProjectedCoords(vec4 atlasRect, vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    projCoords.x = (projCoords.x * atlasRect.z) + atlasRect.x;
    projCoords.y = (projCoords.y * atlasRect.w) + atlasRect.y;
   
    return projCoords;
}

bool projCoordsClip2(vec4 atlasRect, vec2 projCoords)
{
    if (projCoords.x < atlasRect.x 
        || projCoords.x > (atlasRect.x + atlasRect.z) 
        || projCoords.y < atlasRect.y 
        || projCoords.y > (atlasRect.y + atlasRect.w)) {

        return false;
    }

    return true;
}

bool projCoordsClip(vec4 atlasRect, vec3 projCoords)
{
    if (projCoords.z > 1.0 
        || !projCoordsClip2(atlasRect, projCoords.xy)) {

        return false;
    }

    return true;
}

float sampleShadow(vec3 projCoords, float bias)
{
    float depth = texture(shadowDepthAtlas, projCoords.xy).r; 
    return ((projCoords.z - bias) > depth) ? 0.0 : 1.0;
}

float sampleShadow(vec2 uv, float bias, float fragmentDepth)
{
    float depth = texture(shadowDepthAtlas, uv).r; 
    return ((fragmentDepth - bias) > depth) ? 0.0 : 1.0;    
}

float pcfShadowCalculation(vec3 projCoords, float ndotl, vec4 atlasRect)
{
    float currentDepth = projCoords.z;

    float bias = 0.0;//0.0000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    vec2 texelSize = 1.0 / textureSize(shadowDepthAtlas, 0);
    float shadow = 0;
    vec2 uv;
    int div=0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            uv = projCoords.xy + vec2(x,y) * texelSize;            
            if (projCoordsClip2(atlasRect, uv)) 
            {
                shadow += sampleShadow(uv, bias, projCoords.z);    

                div++;  
            }
        }    
    }
    shadow /= div;

    return shadow;
}