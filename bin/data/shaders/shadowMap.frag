#version 450

layout (location=0) out vec4 fragColor;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;
in mat4 vsProjectionViewMatrix;

void main()
{
    vec4 lightSpacePosition = vsProjectionViewMatrix * vsPosition;

    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    fragColor = vec4(projCoords, 1.0);
}
