#version 450

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec4 vsPosition;
out vec3 vsNormal;
out vec2 vsTexcoord;

void main()
{
    vec4 p = modelMatrix * vec4(VertexPosition, 1.0);

    vsPosition = p;
    vsNormal = VertexNormal;
    vsTexcoord = TexCoord;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(VertexPosition,1.0);
}
