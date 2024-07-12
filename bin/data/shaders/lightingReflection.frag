#version 450

#extension GL_ARB_bindless_texture : require

#include include/probeBlock.glsl

layout (location=0) out vec4 fragColor;

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

void main()
{
    vec3 diffuse = vec3(0.7);
    if (hasDiffuseSampler == 1) {
        diffuse = texture(diffuseSampler, gsTexcoord).xyz;
    }

    vec3 normal = gsNormal;
    if (hasNormalSampler == 1) {
        normal = texture(normalSampler, gsTexcoord).xyz;
        normal = gsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 roughness = vec3(0.0);
    if (hasRoughnessSampler == 1) {
        roughness = texture(roughnessSampler, gsTexcoord).xyz;
    }

    vec3 view = normalize(gsPosition.xyz - viewPosition);
    vec3 viewReflection = reflect(view, normal);
    vec3 reflectionColor = calculateReflectionColorFromEnvironmentProbes(gsPosition.xyz, viewReflection, roughness, normal);

    fragColor = vec4(reflectionColor, 1.0);
}
