#version 450 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
//    vec4 value = texture(texture1, TexCoord);

    vec3 value = vec3(
        textureGather(texture1, TexCoord, 0).x,
        textureGather(texture1, TexCoord, 1).x,
        textureGather(texture1, TexCoord, 2).x
    );

    FragColor = vec4(vec3(value.xyz), 1.0);
}