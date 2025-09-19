#version 450

#extension GL_ARB_bindless_texture : require

#include include/probeBlock.glsl

layout (location=0) out vec4 fragColor;

uniform vec3 viewPosition;
uniform int hasDiffuseSampler;

layout(bindless_sampler) uniform sampler2D diffuseSampler;
layout(bindless_sampler) uniform samplerCube environmentSampler;

in vec3 vsNormal;
in vec4 vsPosition;
in vec2 vsTexcoord;
in mat3 vsInvTBN;

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
        diffuse = texture(diffuseSampler, vsTexcoord).xyz;
    }

    vec3 fragmentWorldPos = vsPosition.xyz;
    vec3 probeColor = vec3(diffuse.xyz);

    vec3 selectedColor[4];
    float selectedWeights[4];
    int numSelectedProbes = 0;

    for (int probeIndex = 0; probeIndex < EnvironmentProbeStorageBuffer_size; probeIndex++) {
        if (numSelectedProbes > 3) {
            break;
        }

        EnvProbeStructure probe = probes[probeIndex];

        if (fragmentWorldPos.x < probe.boundingBoxMin.x || fragmentWorldPos.y < probe.boundingBoxMin.y || fragmentWorldPos.z < probe.boundingBoxMin.z) {
            continue;
        }

        if (fragmentWorldPos.x > probe.boundingBoxMax.x || fragmentWorldPos.y > probe.boundingBoxMax.y || fragmentWorldPos.z > probe.boundingBoxMax.z) {
            continue;
        }

        vec3 ray = fragmentWorldPos - probe.position;
        float l = length(ray);
        ray = normalize(ray);

        vec3 planeIntersect1 = (probe.boundingBoxMax.xyz - probe.position) / ray;
        vec3 planeIntersect2 = (probe.boundingBoxMin.xyz - probe.position) / ray;
        vec3 furthestPlane = max(planeIntersect1, planeIntersect2);
        float d = min(min(furthestPlane.x, furthestPlane.y), furthestPlane.z);
        float fl = max(min(1.0 - (l / d), 1.0), 0);

        selectedColor[numSelectedProbes] = probe.debugColor;
        selectedWeights[numSelectedProbes] = fl;
        numSelectedProbes++;
    }

    float sumWeights = 0;
    float invSumWeights = 0;
    for (int i = 0; i < numSelectedProbes; i++) {
        sumWeights += selectedWeights[i];
        invSumWeights += (1.0 - selectedWeights[i]);
    }

    if (numSelectedProbes > 1) {
        float blendFactor[4];
        float sumBlendFactor = 0;

        for (int i = 0; i < numSelectedProbes; i++) {
            blendFactor[i] = ((selectedWeights[i] / sumWeights)) / (numSelectedProbes - 1);
            blendFactor[i] *= ((selectedWeights[i]) / invSumWeights);
            sumBlendFactor += blendFactor[i];
        }
        vec3 reflectionColor = vec3(0);
        for (int i = 0; i < numSelectedProbes; i++) {
            float fl = blendFactor[i] / sumBlendFactor;
            reflectionColor += selectedColor[i] * (selectedWeights[i] / sumWeights);
        }

        probeColor =  reflectionColor;
    } else if (numSelectedProbes == 1) {
        probeColor = selectedColor[0];
    }

    fragColor = vec4(probeColor, 1.0);
}
