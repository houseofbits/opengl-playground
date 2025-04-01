#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main() {
//    FragColor = vec4(1,1,0,1);
    FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
}