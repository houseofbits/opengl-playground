#version 450 core

#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;

layout(bindless_sampler) uniform samplerCube environmentSampler;
uniform vec3 viewPosition;

void main()
{
    vec3 view = normalize(vsPosition.xyz - viewPosition);
    vec3 sky = texture(environmentSampler, view).rgb;

    FragColor = vec4(sky, 1.0);
}