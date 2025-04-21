#version 450 core
#extension GL_ARB_bindless_texture: require

#include include/crosshair.glsl
#include include/fxaa.glsl

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D crosshairTexture;

void main() {
    vec2 screenSize = textureSize(screenTexture, 0);
    vec2 inverseScreenSize = 1.0 / screenSize;

    vec3 color = applyFXAA(screenTexture, TexCoords, inverseScreenSize);
    color = applyCrosshair(crosshairTexture, color, screenSize, TexCoords);

    FragColor = vec4(color, 1.0);
}