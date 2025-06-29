#version 450

#extension GL_ARB_bindless_texture : require

#include include/probeBlock.glsl

const float PI = 3.14159265359;

layout (location=0) out vec4 fragColor;

const int NUM_PCF_SAMPLES = 16;
const vec2 poissonDisk[NUM_PCF_SAMPLES] = {
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
};

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
    float bias;
    float blurRadius;
};

layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};

uniform uint SpotLightStorageBuffer_size;
uniform vec3 viewPosition;

//MaterialResource::bind
uniform float roughnessFactor;
uniform float metallicFactor;
uniform vec3 diffuseColor;
uniform vec3 emissiveColor;
uniform int hasDiffuseSampler;
uniform int hasNormalSampler;
uniform int hasRoughnessSampler;
uniform int hasEmissiveSampler;
uniform int doesReceiveShadows;
uniform int wrappingType;

layout(bindless_sampler) uniform sampler2D diffuseSampler;
layout(bindless_sampler) uniform sampler2D normalSampler;
layout(bindless_sampler) uniform sampler2D roughnessSampler;
layout(bindless_sampler) uniform sampler2D emissiveSampler;
layout(bindless_sampler) uniform samplerCube environmentSampler;
layout(bindless_sampler) uniform sampler2D brdfLUT;

in vec3 vsNormal;
in vec4 vsPosition;
in vec2 vsTexcoord;
in mat3 vsInvTBN;
in vec3 vsTangentViewDirection;

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

float sampleShadow(in sampler2DShadow shadowMap, vec2 uv, float bias, float fragmentDepth)
{
    return texture(shadowMap, vec3(uv, fragmentDepth - bias)).x;
}

float pcssShadowCalculation(SpotLightStructure light, vec3 projCoords, float ndotl)
{
    sampler2DShadow shadowMap = sampler2DShadow(light.shadowSamplerHandle);

    float filterRadiusUV = light.blurRadius / 2048.0; //size;

    float shadow = 0;
    vec2 uv;
    for (int i = 0; i < NUM_PCF_SAMPLES; ++i)
    {
        uv = projCoords.xy + poissonDisk[i] * filterRadiusUV;
        shadow += sampleShadow(shadowMap, uv, light.bias, projCoords.z);
    }

    return shadow / NUM_PCF_SAMPLES;
}

vec3 triplanarDiffuseTexture(in sampler2D diffuseSampler, vec3 surfaceNormal)
{
    vec3 blendWeights = abs(surfaceNormal);
    blendWeights /= (blendWeights.x + blendWeights.y + blendWeights.z);

    vec3 xaxis = texture2D( diffuseSampler, vsPosition.yz).rgb;
    vec3 yaxis = texture2D( diffuseSampler, vsPosition.xz).rgb;
    vec3 zaxis = texture2D( diffuseSampler, vsPosition.xy).rgb;

    return xaxis * blendWeights.x + yaxis * blendWeights.y + zaxis * blendWeights.z;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec2 parallaxMapping(in sampler2D depthMap, vec2 texCoords, vec3 viewDir)
{
    float heightScale = 0.1;

    float height =  texture(depthMap, texCoords).r;
    return texCoords - viewDir.xy * (height * heightScale);
}

void main()
{
    vec3 viewDepth = vsPosition.xyz - viewPosition;
    vec3 view = normalize(viewDepth);

    vec2 texCoords = vsTexcoord;
//    vec2 texCoords = parallaxMapping(roughnessSampler, vsTexcoord, normalize(vsTangentViewDirection));

    vec3 diffuse = diffuseColor;
    if (hasDiffuseSampler == 1) {
        if (wrappingType == 0) {
            diffuse = texture(diffuseSampler, texCoords).xyz;
        } else {
            diffuse = triplanarDiffuseTexture(diffuseSampler, vsNormal);
        }
    }

    vec3 normal = normalize(vsNormal);
    if (hasNormalSampler == 1) {
        normal = texture(normalSampler, texCoords).xyz;
        normal = vsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 emissive = emissiveColor;
    if (hasEmissiveSampler == 1) {
        emissive = texture(emissiveSampler, texCoords).xyz;
    }

    float roughness = roughnessFactor;
    float metallic = metallicFactor;
    if (hasRoughnessSampler == 1) {
        vec2 metallicRoughness = texture(roughnessSampler, texCoords).gb;
        roughness = metallicRoughness.x;
        metallic = metallicRoughness.y;
    }

    vec3 viewReflection = reflect(view, normal);
    vec3 reflectionColor = calculateReflectionColorFromEnvironmentProbes(vsPosition.xyz, viewReflection, roughness, normal, environmentSampler);

    vec3 color = vec3(0.0);
    vec3 falloff;
    vec3 lightDir;
    float distToLight;
    vec3 lightColor = vec3(0);

    vec3 diffuseSpecular = diffuse.rgb + reflectionColor;
    vec3 surfaceNormal = normalize(vsNormal);

    //IBL
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, diffuse, metallic);
    vec3 V = normalize(viewPosition - vsPosition.xyz);

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

            float ndotlSurf = dot(lightDir, surfaceNormal);

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
                shadow = pcssShadowCalculation(light, lightProjectedPosition, dot(lightDir, vsNormal)); //depth > lightProjectedPosition.z;
            }

            vec3 diffuseLight = ndotl
                * light.color
                * light.intensity
                * falloff;

            //IBL
            vec3 L = lightDir;
            vec3 H = normalize(V + L);

            float NDF = DistributionGGX(normal, H, roughness);
            float G   = GeometrySmith(normal, V, L, roughness);
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

            vec3 numerator    = NDF * G * F;
            float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
            vec3 specular = numerator / denominator;

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;

            lightColor += (kD * diffuse / PI + specular) * attenuation * shadow * diffuseLight;
        }
    }

    //Ambient IBL
    float NdotV = max(dot(normal, V), 0.0);
    vec2 brdf = texture2D(brdfLUT, vec2(NdotV, roughness)).xy;
    vec3 F = fresnelSchlickRoughness(NdotV, F0, roughness);
    vec3 specular = reflectionColor * (F * brdf.x + brdf.y);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 lightedColor = emissive * diffuse;
    lightedColor = (kD * lightedColor + specular);// * ao;
    lightedColor += lightColor;

    //Add fog
    vec3 fog = textureLod(environmentSampler, view, 5).rgb;
    float zd = min(1.0, length(viewDepth) / 20.0);
    vec3 lightFogColor = lightedColor + (fog * (zd * zd));

    fragColor = vec4(lightFogColor, 1.0);
}
