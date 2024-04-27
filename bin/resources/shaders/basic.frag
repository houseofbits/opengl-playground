#version 450

layout (location=0) out vec4 fragColor;
// layout (location = 0) out float fragmentDepth;

out vec4 position;
in vec3 normal;
in vec3 uv;

void main()
{
    // float l = length(gsPosition) / gsFarPlane;
    fragColor = vec4(normal, 1.0);
}
