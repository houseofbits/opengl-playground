#version 450

#extension GL_ARB_bindless_texture : require

#include include/probeBlock.glsl

layout (location=0) out vec4 fragColor;

struct SpotLightStructure {
    vec3 color;
    float intensity;
    vec3 position;
    float attenuation;
    mat4 projectionViewMatrix;
    vec3 direction;
    int isPointSource;
    uvec2 projectorSamplerHandle;
    uvec2 _PLACEHOLDER1;
    uvec2 shadowSamplerHandle;
    uvec2 _PLACEHOLDER2;
};

layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};

uniform uint SpotLightStorageBuffer_size;
uniform vec3 viewPosition;
uniform int hasDiffuseSampler;
uniform int hasNormalSampler;
uniform int hasRoughnessSampler;

layout(bindless_sampler) uniform sampler2D diffuseSampler;
layout(bindless_sampler) uniform sampler2D normalSampler;
layout(bindless_sampler) uniform sampler2D roughnessSampler;

in vec3 gsNormal;
in vec4 gsPosition;
in vec2 gsTexcoord;
in mat3 gsInvTBN;

bool isSamplerHandleValid(uvec2 handle) {
    return (handle.x != 0 || handle.y != 0);
}

vec3 calculateProjectedCoords(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    return projCoords;
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

float sampleShadow(in sampler2D shadowMap, vec2 uv, float bias, float fragmentDepth)
{
    float depth = texture(shadowMap, uv).z;
    return ((fragmentDepth - bias) > depth) ? 0.0 : 1.0;
}

float pcfShadowCalculation(SpotLightStructure light, vec3 projCoords, float ndotl)
{
    sampler2D shadowMap = sampler2D(light.shadowSamplerHandle);
    float bias = 0.0001 + (ndotl * 0.0001);
    float blurFactor = (1.0 / 800.0); //textureSize(shadowDepthAtlas, 0).x;  //
    float shadow = 0;
    vec2 uv;
    int div=0;
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            uv = projCoords.xy + vec2(x, y) * blurFactor;
            if (isProjCoordsClipping(uv))
            {
                shadow += sampleShadow(shadowMap, uv, bias, projCoords.z);

                div++;
            }
        }
    }
    shadow /= div;

    return shadow;
}

void main()
{
    float specularPower = 1.0;
    vec3 specularLevel = vec3(1.0);

    vec3 diffuse = vec3(0.7);
    if (hasDiffuseSampler == 1) {
        diffuse = texture(diffuseSampler, gsTexcoord).xyz;
    }

    vec3 normal = gsNormal;
    if (hasNormalSampler == 1) {
        normal = texture(normalSampler, gsTexcoord).xyz;
        normal = gsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 roughness = vec3(1.0);
    if (hasRoughnessSampler == 1) {
        roughness = texture(roughnessSampler, gsTexcoord).xyz;
    }

    vec3 view = normalize(gsPosition.xyz - viewPosition);
    vec3 viewReflection = reflect(view, normal);
    float frensnel = pow(1.0 - dot(normal, -view), 2);

    vec3 reflectionColor = calculateReflectionColorFromEnvironmentProbes(gsPosition.xyz, viewReflection, roughness, normal)
     * (1.0 - roughness)
     * frensnel;

    vec3 color = vec3(0.0);
    vec3 falloff;
    vec3 lightDir;
    float distToLight;
    vec3 lightColor = vec3(0.0);    //vec3(selfIllumination) + (diffuse * ambientEnvColor);

    for (int lightIndex = 0; lightIndex < SpotLightStorageBuffer_size; lightIndex++) {
        SpotLightStructure light = spotLights[lightIndex];

        vec4 lightSpacePosition = light.projectionViewMatrix * gsPosition;
        vec3 lightProjectedPosition = calculateProjectedCoords(lightSpacePosition);
        if (isProjCoordsClipping(lightProjectedPosition.xy) && lightProjectedPosition.z > 0.0 && lightProjectedPosition.z < 1.0) {
            if (light.isPointSource == 1) {
                vec3 toLight = light.position - gsPosition.xyz;
                distToLight = length(toLight);
                lightDir = normalize(toLight);
            } else {
                distToLight = dot(normalize(light.direction), light.position - gsPosition.xyz);
                lightDir = normalize(light.direction);
            }

            float ndotlSurf = dot(lightDir, gsNormal);

            if (ndotlSurf < 0) {
                continue;
            }

            float ndotl = dot(lightDir, normal);

            float attenuation = 1.0 - clamp(distToLight / light.attenuation, 0.0, 1.0);

            falloff = vec3(1.0);
            if (isSamplerHandleValid(light.projectorSamplerHandle)) {
                falloff = texture(sampler2D(light.projectorSamplerHandle), lightProjectedPosition.xy).rgb;
            }

            float shadow = 1.0;
            if (isSamplerHandleValid(light.shadowSamplerHandle)) {
                shadow = pcfShadowCalculation(light, lightProjectedPosition, dot(lightDir, gsNormal)); //depth > lightProjectedPosition.z;
            }

            vec3 diffuseLight = diffuse.rgb
                * ndotl
                * light.color
                * light.intensity;

            lightColor += attenuation
                * falloff
                * shadow
                * (diffuseLight + reflectionColor);
        }
    }

    fragColor = vec4(lightColor, 1.0);
}
