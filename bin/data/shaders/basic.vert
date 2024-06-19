#version 450

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;

uniform mat4 viewProjectionMatrix;

out vec4 position;
out vec3 normal;
out vec2 uv;

void main()
{   
    normal = vertexNormal;
    uv = texCoord;

    position = viewProjectionMatrix * vec4(vertexPosition,1.0);
    gl_Position = position;
}
