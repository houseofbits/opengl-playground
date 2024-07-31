#version 450

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec4 vsPosition;
out vec3 vsNormal;
out vec2 vsTexcoord;

void main()
{
    mat3 modelInverse = mat3(transpose(inverse(modelMatrix)));

    vsNormal = modelInverse * vertexNormal;
    vsTexcoord = texCoord;
    vsPosition = viewProjectionMatrix * modelMatrix * vec4(vertexPosition,1.0);

    gl_Position = vsPosition;
}
