#version 450 core
#extension GL_ARB_bindless_texture: require

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D inputTexture;

void main() {
    vec4 color = texture(inputTexture, TexCoords);

    FragColor = color;
}