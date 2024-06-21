#version 450

#extension GL_ARB_bindless_texture : require

layout (location=0) out vec4 fragColor;

struct SpotLightStructure {
    mat4 projectionViewMatrix;
    vec3 color;
    float intensity;
};

layout (std430, binding = ${INDEX_SpotLightStorageBuffer}) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};
uniform uint numSpotLights;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;

layout(bindless_sampler) uniform sampler2D diffuseSampler;

void main()
{
    vec4 diffuse = texture(diffuseSampler, vsTexcoord);

    vec3 lightColor = spotLights[1].color;

    fragColor = vec4(diffuse.rgb * lightColor, 1.0);
}
