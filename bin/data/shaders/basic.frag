#version 450

#extension GL_ARB_bindless_texture : require

layout (location=0) out vec4 fragColor;

in vec4 position;
in vec3 normal;
in vec2 uv;

layout(bindless_sampler) uniform sampler2D diffuseSampler;

void main()
{
    vec4 diffuse = texture(diffuseSampler, uv);

    fragColor = vec4(diffuse.rgb, 1.0);
}
