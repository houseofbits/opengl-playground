#version 450

layout (location=0) out vec4 fragColor;

in vec3 vsNormal;

void main()
{
    fragColor = vec4(vsNormal, 1.0);
}
