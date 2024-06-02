#version 410

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec4 VertexColor;

uniform mat4 viewProjectionMatrix;

out vec4 vsColor;
out vec4 vsPosition;

void main()
{
    vsPosition = vec4(VertexPosition, 1.0);
    vsColor = VertexColor;
    gl_Position = viewProjectionMatrix * vec4(VertexPosition,1.0);
}
