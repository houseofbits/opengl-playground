#version 450

layout (location=0) out vec4 FragColor;

#include include/structures.glsl
#include include/lightBlock.glsl
#include include/textureAtlas.glsl

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;

in vec4 fragmentPositionPerLightView[MAX_LIGHTS];

vec3 calculateProjectedCoords(uint index, vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    projCoords.xy = calculateAtlasUV(index, projCoords.xy);

    return projCoords;
}

float sampleShadow(vec3 projCoords, float bias)
{
    float depth = texture(shadowDepthAtlas, projCoords.xy).r; 
    return projCoords.z - bias > depth ? 0.0 : 1.0;    
}

float sampleShadow(vec2 uv, float bias, float fragmentDepth)
{
    float depth = texture(shadowDepthAtlas, uv).r; 
    return ((fragmentDepth - bias) > depth) ? 0.0 : 1.0;    
}

float pcfShadowCalculation(vec3 projCoords, uint index)
{
    float currentDepth = projCoords.z;

    float bias = 0.0000001;

    vec2 texelSize = 1.0 / textureSize(shadowDepthAtlas, 0);
    float shadow = 0;
    vec2 uv;
    int div=0;
    for(int x = -3; x <= 3; ++x)
    {
        for(int y = -3; y <= 3; ++y)
        {
            uv = projCoords.xy + vec2(x,y) * texelSize;            
            if (isProjCoordsClipping(index, uv)) 
            {
                shadow += sampleShadow(uv, bias, projCoords.z);

                div++;  
            }
        }    
    }
    shadow /= div;

    return shadow;
}

void main() 
{
    float inShadow = 0;
    for(int lightIndex = 0; lightIndex < numActiveLights; lightIndex++) {    
        // uint lightIndex = 0;
        LightStructure light = lights[lightIndex];
        vec3 projCoords = calculateProjectedCoords(light.shadowAtlasIndex, fragmentPositionPerLightView[lightIndex]);
        
        if (isProjCoordsClipping(light.shadowAtlasIndex, projCoords.xy)) {
            // inShadow = sampleShadow(projCoords, 0.0000001);
            inShadow += pcfShadowCalculation(projCoords, light.shadowAtlasIndex);
        }    
    }
    FragColor = vec4(vec3(inShadow / 2.0), 1.0);
}