#version 450

layout (location=0) out vec4 FragColor;

#include include/structures.glsl
#include include/lightBlock.glsl
#include include/lightViewsBlock.glsl

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;
// uniform uint numActiveLights;
uniform sampler2D texture1;
uniform sampler2D lightDepthMap1;

in vec4 fragmentPositionPerLightView[6];

float sampleShadow(vec3 projCoords, float bias)
{
    float depth = texture(lightDepthMap1, projCoords.xy).r; 
    return projCoords.z - bias > depth ? 0.0 : 1.0;    
}

float sampleShadow(vec3 projCoords, float bias, vec2 texelSize, int x, int y)
{
    projCoords.xy = projCoords.xy + vec2(x, y) * texelSize;
    return sampleShadow(projCoords, bias);
}

float pcfShadowCalculation(vec4 fragPosLightSpace, vec4 atlasRect, float ndotl)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) {
        return 0.0;
    }

    projCoords.x = (projCoords.x * atlasRect.z) + atlasRect.x;
    projCoords.y = (projCoords.y * atlasRect.w) + atlasRect.y;

    if (projCoords.x < atlasRect.x || projCoords.x > (atlasRect.x + atlasRect.z) || projCoords.y < atlasRect.y || projCoords.y > (atlasRect.y + atlasRect.w)) {
        return 0.0;
    }

    float currentDepth = projCoords.z;

    float bias = 0.0000001 * tan(acos(clamp(ndotl, 0.0, 1.0)));

    vec2 texelSize = 1.0 / textureSize(lightDepthMap1, 0);
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

vec3 diffuseComponent(vec3 lightDir, vec3 normal, vec3 color)
{
    float ndotl = max(dot(lightDir, normal), 0.0);
    return ndotl * color;
}

vec3 specularComponent(vec3 lightDir, vec3 normal)
{
    vec3 viewDir = normalize(viewPosition - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
 
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    return vec3(1.0) * spec;
}

vec3 calculateSpotLight(Light light, vec4 fragPosLightSpace, vec4 atlasRect)
{
    vec3 toLight = light.position - fs_in.FragPos;
    float dist = length(toLight);

    if (dist > light.distAttenMax) {
        return vec3(0.0);
    }    

    vec3 lightDir = normalize(toLight);
    vec3 normal = normalize(fs_in.Normal);
    float ndotl = dot(lightDir, normal);
    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

    float shadow = pcfShadowCalculation(fragPosLightSpace, atlasRect, ndotl);
    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

    float angle = acos(dot(-lightDir, light.direction));
    float halfBeamAngle = light.beamAngle * 0.5;

    if (angle > halfBeamAngle) {
        return vec3(0.0);
    }

    float falloff = (halfBeamAngle - angle) / halfBeamAngle;
    float attenuation = 1.0 - clamp( dist / light.distAttenMax, 0.0, 1.0); 
    vec3 diffuse =  max(ndotl, 0.0) * light.color;
    vec3 specular = specularComponent(lightDir, normal);

    return vec3(shadow) * light.intensity * falloff * attenuation * (diffuse + specular); 
}

void main() 
{
    vec3 textureColor = texture(texture1, fs_in.TexCoord).xyz;

    vec3 lightColor = vec3(0.0);
    Light light;
    for(int index = 0; index < numActiveLights; index++)
    {   
        vec4 fragPosLightSpace = fragmentPositionPerLightView[index];
        vec4 atlasRect = shadowFragments[index].atlasRect;        
        light = lights[index];

        lightColor += calculateSpotLight(light, fragPosLightSpace, atlasRect);
    }

    FragColor = vec4(textureColor * lightColor, 1.0);
    // FragColor = vec4(lightColor, 1.0);
}