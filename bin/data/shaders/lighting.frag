#version 450

#extension GL_ARB_bindless_texture : require

layout (location=0) out vec4 fragColor;

struct EnvProbeStructure {
    vec3 position;
    uint cubeMapTextureLayer;
    vec4 boundingBoxMin;
    vec4 boundingBoxMax;
    vec3 debugColor;
};

struct SpotLightStructure {
    vec3 color;
    float intensity;
    vec3 position;
    float attenuation;
    mat4 projectionViewMatrix;
    vec3 direction;
    int projectorSamplerIndex;
    int isPointSource;
};


layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};
uniform uint numSpotLights;

layout (binding = ${INDEX_EnvironmentProbeStorageBuffer}, std430) readonly buffer EnvironmentProbeStorageBuffer {
    EnvProbeStructure probes[100];
};
uniform uint numEnvProbes;

layout(binding = ${INDEX_SamplerIndexStorageBuffer}, std430) readonly buffer SamplerIndexStorageBuffer {
    sampler2D projectorSamplers[];
};

in vec3 gsNormal;
in vec4 gsPosition;
in vec2 gsTexcoord;
in mat3 gsInvTBN;

layout(bindless_sampler) uniform sampler2D diffuseSampler;
layout(bindless_sampler) uniform sampler2D normalSampler;

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
    vec4 diffuse = texture(diffuseSampler, gsTexcoord);
    vec3 normal = texture(normalSampler, gsTexcoord).xyz;

    normal = gsInvTBN * normalize(normal * 2.0 - 1.0);

//    uint lightIndex = 0;

    vec3 color = vec3(0.0);
    vec3 falloff;
    vec3 lightDir;
    float distToLight;

    for (int lightIndex = 0; lightIndex < numSpotLights; lightIndex++) {
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

            if (light.projectorSamplerIndex >= 0) {
                falloff = texture(projectorSamplers[light.projectorSamplerIndex], lightProjectedPosition.xy).rgb;
            } else {
                falloff = vec3(1.0);
            }

            color += light.intensity * ndotl * light.color * attenuation * falloff;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // probes
    vec3 fragmentWorldPos = gsPosition.xyz;
    vec3 probeColor = vec3(0);

    vec3 selectedColor[4];
    float selectedWeights[4];
    int numSelectedProbes = 0;

    for (int probeIndex = 0; probeIndex < numEnvProbes; probeIndex++) {
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
//            reflectionColor = mix(reflectionColor, selectedColor[i], fl);
            reflectionColor += selectedColor[i] * (selectedWeights[i] / sumWeights);
        }

        probeColor =  reflectionColor;
    } else if (numSelectedProbes == 1) {
        probeColor = selectedColor[0];
    }


//    EnvProbeStructure probe = probes[0];

    fragColor = vec4(probeColor, 1.0);

//    fragColor = vec4(color * diffuse.rgb, 1.0);
}
