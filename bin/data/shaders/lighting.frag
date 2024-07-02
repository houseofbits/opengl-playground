#version 450

#extension GL_ARB_bindless_texture : require

layout (location=0) out vec4 fragColor;

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

layout(binding = ${INDEX_ProjectionSamplerStorageBuffer}, std430) readonly buffer ProjectionSamplerStorageBuffer {
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
                falloff = vec3(1.0 - length(lightSpacePosition.xy / lightSpacePosition.w));
            }

            color += light.intensity * ndotl * light.color * attenuation * falloff;
        }
    }

    fragColor = vec4(color * diffuse.rgb, 1.0);
}
