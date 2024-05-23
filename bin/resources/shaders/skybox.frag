#version 450 core

out vec4 FragColor;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;

uniform samplerCube skyboxTexture;
uniform vec3 viewPosition;

void main()
{
    vec3 view = normalize(vsPosition.xyz - viewPosition);
    vec3 sky = texture(skyboxTexture, view).rgb;

    FragColor = vec4(sky, 1.0);
}