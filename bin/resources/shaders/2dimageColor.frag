#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    vec4 value = texture(texture1, TexCoord);

    FragColor = vec4(vec3(value.xyz), 1.0);
}