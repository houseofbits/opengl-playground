#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec2 aTexCoord;

out vec4 vsPosition;
out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
    vsPosition = vec4(aPosition, 1.0);

    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}