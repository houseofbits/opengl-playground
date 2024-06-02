
struct EnvProbeStructure {
    vec3 position;
    uint cubeMapTextureLayer;
    vec4 boundingBoxMin;
    vec4 boundingBoxMax;
};

layout (std140, binding = 4) uniform ProbesBlock {
    EnvProbeStructure probes[32];
};

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