

struct EnvProbeStructure {
    vec3 position;
    uint cubeMapTextureLayer;
    vec4 boundingBoxMin;
    vec4 boundingBoxMax;
};

layout (std140, binding = 4) uniform ProbesBlock {
    EnvProbeStructure probes[32];
};

uniform samplerCubeArray envProbeArray;
uniform uint numProbes;

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

vec3 ambientReflection(
    EnvProbeStructure probe,
    vec3 fragmentWorldPos,
    vec3 normal) {
    vec3 lookup = normalize(fragmentWorldPos - probe.position);
    return reflect(lookup, normal);
}

int roughnessToLod(float roughness)
{
    return int(roughness * 5.0);
}

vec3 calculateReflectionColorFromEnvironmentProbes(vec3 fragmentWorldPos, vec3 viewReflection, float roughness)
{
    vec3 selectedColor[4];
    float selectedWeights[4];
    int numSelectedProbes = 0;

    for (int probeIndex = 0; probeIndex < numProbes; probeIndex++) {
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
        vec3 planeIntersect1 = (probe.boundingBoxMax.xyz - probe.position) / ray;
        vec3 planeIntersect2 = (probe.boundingBoxMin.xyz - probe.position) / ray;
        vec3 furthestPlane = max(planeIntersect1, planeIntersect2);
        float d = min(min(furthestPlane.x, furthestPlane.y), furthestPlane.z);
        float fl = max(min(1.0 - (l / d), 1.0), 0);

        selectedColor[numSelectedProbes] = textureLod(
                envProbeArray,
                vec4(reflectedRay, probe.cubeMapTextureLayer),
                roughnessToLod(roughness)).rgb;

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
}

vec3 calculateAmbientColorFromEnvironmentProbes(vec3 fragmentWorldPos, vec3 normal, vec3 faceNormal)
{
    vec3 ambientEnvColor = vec3(0.0);
    vec3 n = normalize(normal + faceNormal * 5);
    vec3 selectedColor[10];
    float selectedWeight[10];
    int numSelectedProbes = 0;
    for (int probeIndex = 0; probeIndex < numProbes; probeIndex++) {
        if (probeIndex >= 10) {
            break;
        }
        EnvProbeStructure probe = probes[probeIndex];
        vec3 reflectedRay = ambientReflection(probe, gsPosition.xyz, n);
        selectedWeight[numSelectedProbes] = length(gsPosition.xyz - probe.position);
        selectedColor[numSelectedProbes] = textureLod(envProbeArray, vec4(reflectedRay, probe.cubeMapTextureLayer), 4).rgb;
        numSelectedProbes++;
    }
    float sumWeights = 0;
    for(int i=0; i<numSelectedProbes; i++) {
        sumWeights += selectedWeight[i];
    }

    for(int i=0; i<numSelectedProbes; i++) {
        ambientEnvColor += selectedColor[i] * (selectedWeight[i] / sumWeights);
    }

    return ambientEnvColor;
}