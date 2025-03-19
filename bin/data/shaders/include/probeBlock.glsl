

struct EnvProbeStructure {
    vec3 position;
    int cubeMapTextureLayer;
    vec4 boundingBoxMin;
    vec4 boundingBoxMax;
    vec3 debugColor;
};

layout (binding = ${INDEX_EnvironmentProbeStorageBuffer}, std430) readonly buffer EnvironmentProbeStorageBuffer {
    EnvProbeStructure probes[100];
};

layout(bindless_sampler) uniform samplerCubeArray probesCubeArraySampler;
uniform uint EnvironmentProbeStorageBuffer_size;


vec3 parallaxCorrectedProbeReflection(
    EnvProbeStructure probe,
    vec3 fragmentWorldPos,
    vec3 viewReflection) {

    vec3 planeIntersect1 = (probe.boundingBoxMax.xyz - fragmentWorldPos) / viewReflection;
    vec3 planeIntersect2 = (probe.boundingBoxMin.xyz - fragmentWorldPos) / viewReflection;

    vec3 furthestPlane = max(planeIntersect1, planeIntersect2);

    float dist = min(min(furthestPlane.x, furthestPlane.y), furthestPlane.z);

    vec3 intersectedWorldSpacePos = fragmentWorldPos + viewReflection * dist;

   return intersectedWorldSpacePos - probe.position;
}

int roughnessToLod(float roughness)
{
    return int(roughness * 5.0);
}

vec3 calculateReflectionColorFromEnvironmentProbes(vec3 fragmentWorldPos, vec3 viewReflection, vec3 roughness, vec3 normal, in samplerCube environmentSampler)
{
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

        vec3 reflectedRay = parallaxCorrectedProbeReflection(probe, fragmentWorldPos, viewReflection);
        vec3 ray = fragmentWorldPos - probe.position;
        float l = length(ray);
        ray = normalize(ray);
        if (dot(ray, normal) > 0) {
            continue;
        }
        vec3 planeIntersect1 = (probe.boundingBoxMax.xyz - probe.position) / ray;
        vec3 planeIntersect2 = (probe.boundingBoxMin.xyz - probe.position) / ray;
        vec3 furthestPlane = max(planeIntersect1, planeIntersect2);
        float d = min(min(furthestPlane.x, furthestPlane.y), furthestPlane.z);
        float fl = max(min(1.0 - (l / d), 1.0), 0);

        vec4 color = textureLod(
                probesCubeArraySampler,
                vec4(reflectedRay, probe.cubeMapTextureLayer),
                roughnessToLod(length(roughness))
        );

        if (color.a == 0) {
            continue;
        }

        selectedColor[numSelectedProbes] = color.rgb;
        selectedWeights[numSelectedProbes] = fl;
        numSelectedProbes++;
    }

    float sumWeights = 0;
    float invSumWeights = 0;
    for (int i = 0; i < numSelectedProbes; i++) {
        sumWeights += selectedWeights[i];
        invSumWeights += (1.0 - selectedWeights[i]);
    }

    float blendFactor[4];
    float sumBlendFactor = 0;

    if (numSelectedProbes > 1) {
        for (int i = 0; i < numSelectedProbes; i++) {
            blendFactor[i] = ((selectedWeights[i] / sumWeights)) / (numSelectedProbes - 1);
            blendFactor[i] *= ((selectedWeights[i]) / invSumWeights);
            sumBlendFactor += blendFactor[i];
        }
        vec3 reflectionColor = vec3(0);
        for (int i = 0; i < numSelectedProbes; i++) {
            float fl = blendFactor[i] / sumBlendFactor;
            reflectionColor = mix(reflectionColor, selectedColor[i], fl);
        }

        return reflectionColor;
    } else if (numSelectedProbes == 1) {
        return selectedColor[0];
    }

    return texture(environmentSampler, viewReflection).rgb;
}
