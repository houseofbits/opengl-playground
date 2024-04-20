#version 450

layout (location=0) out vec4 FragColor;

#include include/structures.glsl
#include include/lightBlock.glsl
#include include/lightViewsBlock.glsl

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = 1.57079632679489661923;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;
// uniform uint numActiveLights;
uniform sampler2D texture1;
uniform sampler2D shadowDepthAtlas;

in vec4 fragmentPositionPerLightView[6];

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

vec3 getProjectedCoords(LightViewStructure lightView, vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    projCoords.x = (projCoords.x * lightView.atlasSize.x) + lightView.atlasPos.x;
    projCoords.y = (projCoords.y * lightView.atlasSize.y) + lightView.atlasPos.y;

    return projCoords;
}

bool projCoordsClip(LightViewStructure lightView, vec3 projCoords)
{
    if(projCoords.z > 1.0) {
        return false;
    }

    if (projCoords.x < lightView.atlasPos.x 
        || projCoords.x > (lightView.atlasPos.x + lightView.atlasSize.x) 
        || projCoords.y < lightView.atlasPos.y 
        || projCoords.y > (lightView.atlasPos.y + lightView.atlasSize.y)) {

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

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}


float rawShadow2(vec4 fragPosLightSpace, vec2 atlasPos, vec2 atlasSize, float ndotl)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) {
        return 0.0;
    }

    vec2 uv = projCoords.xy;
    uv.x = (uv.x * atlasSize.x) + atlasPos.x;
    uv.y = (uv.y * atlasSize.y) + atlasPos.y;


    if (uv.x < atlasPos.x || uv.x > (atlasPos.x + atlasSize.x) || uv.y < atlasPos.y || uv.y > (atlasPos.y + atlasSize.y)) {
        return 0.0;
    }

    float currentDepth = projCoords.z;
    float depth = texture(shadowDepthAtlas, uv).r;

    float bias = 0.0000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    float shadow = currentDepth - bias > depth ? 0.0 : 1.0;   

    return pow(pow(depth, 100), 100);
}

float rawShadow(vec3 projCoords, float ndotl)
{
    float currentDepth = projCoords.z;
    float depth = texture(shadowDepthAtlas, projCoords.xy).r;

    float bias = 0.0000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    float shadow = currentDepth - bias > depth ? 0.0 : 1.0;   

    return shadow;//pow(pow(depth, 100), 100);
}

float poissonShadowCalculation(vec3 projCoords, float ndotl)
{
    float currentDepth = projCoords.z;

    float bias = 0.000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    float shadow = 1.0;
    for (int i = 0;i < 16;i++){
        int index = i;  //int(random(fs_in.FragPos.xyz, i) * 1000)%16;

        float depth = texture(shadowDepthAtlas, projCoords.xy + poissonDisk[index]/700.0).r; 

        if ( depth  <  currentDepth - bias ){
            shadow -= 0.0625;
        }
    }

    return shadow;
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

void main() 
{
    //Project shadowMap atlas onto scene

    uint index = 1;

    float finalShadow = 0.0;

    vec3 normal = normalize(fs_in.Normal);
    
    vec3 color = vec3(0.0);

    for (uint index = 0; index < numberOfLightViews; index++)
    {
        vec4 fragPosLightSpace = fragmentPositionPerLightView[index];
        LightViewStructure lightView = lightViews[index];
        vec3 lightPosition = lights[index].position;

        vec3 toLight = lightPosition - fs_in.FragPos;
        vec3 lightDir = normalize(toLight);
        float ndotl = dot(lightDir, normal);

        // if (ndotl < 0.0) {
        //     finalShadow = 0.0;
        //     continue;
        // }

        vec3 projCoords = getProjectedCoords(lightView, fragPosLightSpace);
        // if (!projCoordsClip(lightView, projCoords)) {
        //     continue;
        // }

        // finalShadow += poissonShadowCalculation(projCoords ndotl);
        // finalShadow += pcfShadowCalculation(projCoords, ndotl);
        // finalShadow += pcfOffsetShadowCalculation(projCoords, ndotl);
        finalShadow = rawShadow(projCoords, ndotl);     
        // finalShadow = rawShadow2(fragPosLightSpace, lightView.atlasPos, lightView.atlasSize, ndotl);
    }
    FragColor = vec4(vec3(finalShadow), 1.0);
    // FragColor = vec4(color, 1.0);
}