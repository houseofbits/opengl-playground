#version 450

layout(location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 modelMatrix;

out vec4 vsPosition;
out vec3 vsNormal;

void main(){
    vsNormal = vertexNormal;
    vsPosition = modelMatrix * vec4(vertexPosition, 1.0);

    gl_Position = vsPosition;
}
