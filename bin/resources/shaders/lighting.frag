#version 450

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = 1.57079632679489661923;

#define POINT_LIGHT 0 
#define SPOT_LIGHT 1
#define SPOT_LIGHT_WITH_SHADOW 4

const int MAX_LIGHTS = 10; 

struct Light {
    vec3 position;
    float falloff;    

    vec3 color;
    float intensity;

    vec3 direction;
    uint type;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;    
};

layout (location=0) out vec4 FragColor;

layout (std140, binding = 1) uniform LightBlock {
    Light lights[MAX_LIGHTS];
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;
uniform uint numActiveLights;
uniform sampler2D texture1;
uniform sampler2D lightDepthMap1;

float rawShadowCalculation(vec4 fragPosLightSpace, float ndotl)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) {
        return 0.0;
    }

    float closestDepth = texture(lightDepthMap1, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.000005 * (1.0 - ndotl), 0.0000005);  
    float shadow = (currentDepth - bias) > closestDepth  ? 0.0 : 1.0; 

    return shadow;
} 

float pcfShadowCalculation(vec4 fragPosLightSpace, float ndotl)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) {
        return 0.0;
    }

    float currentDepth = projCoords.z;

    float bias = max(0.000005 * (1.0 - ndotl), 0.0000005);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(lightDepthMap1, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(lightDepthMap1, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;        
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

vec3 calculatePointLight(Light light)
{
    vec3 toLight = light.position - fs_in.FragPos;
    float dist = length(toLight);

    if (dist > light.distAttenMax) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(toLight);
    vec3 normal = normalize(fs_in.Normal);
    vec3 diffuse =  diffuseComponent(lightDir, normal, light.color);
    vec3 specular = specularComponent(lightDir, normal);

    float attenuation = 1.0 - clamp( dist / light.distAttenMax, 0.0, 1.0); 
    
    return light.intensity * attenuation * (diffuse + specular);   
}

vec3 calculateSpotLight(Light light, bool withShadows)
{
    vec3 toLight = light.position - fs_in.FragPos;
    float dist = length(toLight);

    if (dist > light.distAttenMax) {
        return vec3(0.0);
    }    

    vec3 lightDir = normalize(toLight);
    float angle = (1.0 - dot(-lightDir, light.direction)) * PI;

    if (angle > light.beamAngle) {
        return vec3(0.0);
    }

    float falloff = (light.beamAngle - angle) / light.beamAngle;
    float attenuation = 1.0 - clamp( dist / light.distAttenMax, 0.0, 1.0); 

    vec3 normal = normalize(fs_in.Normal);
    float ndotl = max(dot(lightDir, normal), 0.0);

    vec3 diffuse =  ndotl * light.color;
    vec3 specular = specularComponent(lightDir, normal);

    float shadow = 1.0;
    if (withShadows) {
        shadow = pcfShadowCalculation(fs_in.FragPosLightSpace, ndotl);
    }

    return shadow * light.intensity * falloff * attenuation * (diffuse + specular); 
}

void main() 
{
    vec3 textureColor = texture(texture1, fs_in.TexCoord).xyz;

    vec3 lightColor = vec3(0.0);
    Light light;
    for(int i=0; i < numActiveLights; i++)
    {   
        light = lights[i];
        if (light.type == POINT_LIGHT) {
            lightColor += calculatePointLight(light);
        } else if(light.type == SPOT_LIGHT) {
            lightColor += calculateSpotLight(light, false);
        } else if(light.type == SPOT_LIGHT_WITH_SHADOW) {
            lightColor += calculateSpotLight(light, true);
        }
    }

    FragColor = vec4(textureColor * lightColor, 1.0);
}