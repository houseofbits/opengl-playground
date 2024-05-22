#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    vec4 value = texture(texture1, TexCoord);
    float depth = pow(pow(value.r, 5), 100);

    FragColor = vec4(vec3(depth), 1.0);
}