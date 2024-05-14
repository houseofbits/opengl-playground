#version 450

layout (location=0) out vec4 fragColor;

in vec4 position;
in vec3 normal;
in vec2 uv;

uniform sampler2D textureId;

void main()
{
    vec4 color = texture(textureId, uv);
    fragColor = vec4(color.rgb, 1.0);
}
