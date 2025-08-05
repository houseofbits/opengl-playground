#version 450

#extension GL_ARB_bindless_texture : require

layout (location=0) out vec4 fragColor;

#include include/lightsBlock.glsl
//
//struct SpotLightStructure {
//    vec3 color;
//    float intensity;
//    vec3 position;
//    float attenuation;
//    mat4 projectionViewMatrix;
//    vec3 direction;
//    int isPointSource;
//    uvec2 projectorSamplerHandle;
//    uvec2 _PLACEHOLDER1;
//    uvec2 shadowSamplerHandle;
//    float bias;
//    float _PLACEHOLDER2;
//};
//
//layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
//    SpotLightStructure spotLights[100];
//};

in vec3 vsNormal;
in vec4 vsPosition;
in vec2 vsTexcoord;
in mat3 vsInvTBN;

//uniform uint SpotLightStorageBuffer_size;
uniform int hasDiffuseSampler;
uniform int hasNormalSampler;
uniform int hasRoughnessSampler;
uniform float selfIllumination;
uniform int doesReceiveShadows;

layout(bindless_sampler) uniform sampler2D diffuseSampler;
layout(bindless_sampler) uniform sampler2D normalSampler;
layout(bindless_sampler) uniform sampler2D roughnessSampler;

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

float pcssShadowCalculation(SpotLightStructure light, vec3 projCoords, float ndotl)
{
    sampler2DShadow shadowMap = sampler2DShadow(light.shadowSamplerHandle);

    return texture(shadowMap, vec3(projCoords.xy, projCoords.z - light.bias)).x;
}

void main()
{
    vec3 diffuse = vec3(0.7);
    if (hasDiffuseSampler == 1) {
        diffuse = texture(diffuseSampler, vsTexcoord).xyz;
    }

    vec3 normal = vsNormal;
    if (hasNormalSampler == 1) {
        normal = texture(normalSampler, vsTexcoord).xyz;
        normal = vsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 color = vec3(0.0);
    vec3 falloff;
    vec3 lightDir;
    float distToLight;

    for (int lightIndex = 0; lightIndex < SpotLightStorageBuffer_size; lightIndex++) {
        SpotLightStructure light = spotLights[lightIndex];

        vec4 lightSpacePosition = light.projectionViewMatrix * vsPosition;
        vec3 lightProjectedPosition = calculateProjectedCoords(lightSpacePosition);
        if (isProjCoordsClipping(lightProjectedPosition.xy) && lightProjectedPosition.z > 0.0 && lightProjectedPosition.z < 1.0) {
            if (light.isPointSource == 1) {
                vec3 toLight = light.position - vsPosition.xyz;
                distToLight = length(toLight);
                lightDir = normalize(toLight);
            } else {
                distToLight = dot(normalize(light.direction), light.position - vsPosition.xyz);
                lightDir = normalize(light.direction);
            }

            float ndotlSurf = dot(lightDir, vsNormal);

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
            if (doesReceiveShadows == 1 && isSamplerHandleValid(light.shadowSamplerHandle)) {
                shadow = pcssShadowCalculation(light, lightProjectedPosition, dot(lightDir, vsNormal));
            }

            color += shadow * light.intensity * 0.3 * ndotl * light.color * attenuation * falloff * diffuse;
        }
    }

    fragColor = vec4(mix(color, diffuse, selfIllumination), 1.0);
}
