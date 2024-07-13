#version 450

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TexCoord;

uniform mat4 viewProjectionMatrix;
uniform vec3 viewPosition;

out vec4 vsPosition;
out vec3 vsNormal;
out vec2 vsTexcoord;

void main()
{
    vec4 p = vec4(viewPosition + VertexPosition, 1.0);
    vec3 n = VertexNormal;

    vsPosition = p;
    vsNormal = n;
    vsTexcoord = TexCoord;

    gl_Position = viewProjectionMatrix * p;
}
