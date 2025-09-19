#version 450

#include include/lightsBlock.glsl

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform int lightIndex;

void main()
{
    mat4 viewProjectionMatrix = spotLights[lightIndex].projectionViewMatrix;

    mat3 modelInverse = mat3(transpose(inverse(modelMatrix)));

//    vsNormal = modelInverse * vertexNormal;
//    vsTexcoord = texCoord;
    vec4 vsPosition = modelMatrix * vec4(vertexPosition,1.0);
//    vsProjectionViewMatrix = spotLights[lightIndex].projectionViewMatrix;

    gl_Position = viewProjectionMatrix * vsPosition;
}
