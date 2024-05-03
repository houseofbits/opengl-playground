#version 450

layout (location=0) out vec4 fragColor;

out vec4 position;
in vec3 normal;
in vec2 uv;

void main()
{   
    fragColor = vec4(normal, 1.0);
}
