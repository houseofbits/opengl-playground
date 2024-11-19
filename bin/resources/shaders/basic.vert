#version 450

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec4 position;
out vec3 normal;
out vec2 uv;

void main()
{   
    mat3 modelInverse = mat3(transpose(inverse(modelMatrix)));

    vec4 p = modelMatrix * vec4(vertexPosition, 1.0);
    vec3 n = modelInverse * vertexNormal;

    position = p;
    normal = n;
    uv = texCoord;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
