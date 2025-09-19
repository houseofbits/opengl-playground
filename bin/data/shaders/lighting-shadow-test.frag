#version 450

#extension GL_ARB_bindless_texture: require

layout (location = 0) out vec4 fragColor;

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
uniform vec3 diffuseColor;
uniform float selfIllumination;
uniform int hasDiffuseSampler;
uniform int hasNormalSampler;
uniform int hasRoughnessSampler;
uniform int doesReceiveShadows;
uniform int wrappingType;

layout (bindless_sampler) uniform sampler2D diffuseSampler;
layout (bindless_sampler) uniform sampler2D normalSampler;
layout (bindless_sampler) uniform sampler2D roughnessSampler;
layout (bindless_sampler) uniform samplerCube environmentSampler;

in vec3 vsNormal;
in vec4 vsPosition;
in vec2 vsTexcoord;
in mat3 vsInvTBN;

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
//    float depth = texture(shadowMap, uv).x;
    float depth = texture(shadowMap, vec3(uv, fragmentDepth - bias)).x;

    return depth;// ? 0.0 : 1.0;
}

float pcfShadowCalculation(SpotLightStructure light, vec3 projCoords, float ndotl)
{
    sampler2DShadow shadowMap = sampler2DShadow(light.shadowSamplerHandle);
    float bias = light.bias;
    float blurFactor = 0.001;
    float shadow = 0;
    vec2 uv;
    int div = 0;
    for (int x = -3; x <= 3; ++x)
    {
        for (int y = -3; y <= 3; ++y)
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

//https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf
float pcssShadowCalculation(SpotLightStructure light, vec3 projCoords, float ndotl)
{
//    sampler2D shadowMap = sampler2D(light.shadowSamplerHandle);
    sampler2DShadow shadowMap = sampler2DShadow(light.shadowSamplerHandle);

//    float depth = texture(shadowMap, projCoords).x;
////    depth = pow(depth, 200);
//
//    return depth;

    float filterRadiusUV = light.blurRadius / 2048;

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

    vec3 xaxis = texture2D(diffuseSampler, vsPosition.yz).rgb;
    vec3 yaxis = texture2D(diffuseSampler, vsPosition.xz).rgb;
    vec3 zaxis = texture2D(diffuseSampler, vsPosition.xy).rgb;

    return xaxis * blendWeights.x + yaxis * blendWeights.y + zaxis * blendWeights.z;
}

float chebyshevUpperBound(vec2 moments, float t)
{
    float g_MinVariance = 0.000001;

    float p = float(t <= moments.x);
    float variance = moments.y - (moments.x * moments.x);
    variance = max(variance, g_MinVariance);
    float d = t - moments.x;
    float p_max = variance / (variance + d * d);

    return max(p, p_max);
}

float linstep(float min, float max, float v)
{
    return clamp((v - min) / (max - min), 0, 1);
}
float reduceLightBleeding(float p_max, float Amount)
{
    return linstep(Amount, 1, p_max);
}

void main()
{
    vec3 normal = normalize(vsNormal);
    if (hasNormalSampler == 1) {
        normal = texture(normalSampler, vsTexcoord).xyz;
        normal = vsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 viewDepth = vsPosition.xyz - viewPosition;
    vec3 view = normalize(viewDepth);
    vec3 viewReflection = reflect(view, normal);
    float frensnel = pow(1.0 - dot(normal, -view), 4);

    vec3 color = vec3(0.0);
    vec3 falloff;
    vec3 lightDir;
    float distToLight;
    vec3 lightColor = vec3(0);

    int numShadows = 0;
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

            float shadow = 1.0;
            if (doesReceiveShadows == 1 && isSamplerHandleValid(light.shadowSamplerHandle)) {
                sampler2D shadowMap = sampler2D(light.shadowSamplerHandle);

                vec4 shadowMapSample = texture(shadowMap, lightProjectedPosition.xy);
                float depth = shadowMapSample.x;

                //Raw comparison
                //                shadow = float(lightProjectedPosition.z < depth);

                //PCF
//                shadow = pcfShadowCalculation(light, lightProjectedPosition, dot(lightDir, vsNormal));
                shadow = pcssShadowCalculation(light, lightProjectedPosition, dot(lightDir, vsNormal));

                //VSM
//                                shadow = chebyshevUpperBound(shadowMapSample.xy, lightProjectedPosition.z);
//                                shadow = reduceLightBleeding(shadow, 0.3);

                //ESM
//                float c = 5000;
//                shadow = clamp(exp( c * (depth - lightProjectedPosition.z)), 0.0, 1.0 );
            }

            lightColor += shadow;
            numShadows++;
        }
    }

    fragColor = vec4(lightColor / numShadows, 1.0);
}
