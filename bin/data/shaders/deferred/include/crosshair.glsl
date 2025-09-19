vec3 applyCrosshair(in sampler2D crosshairTexture, vec3 color, vec2 screenSize, vec2 texCoord) {
    vec2 crossSize = textureSize(crosshairTexture, 0);

    vec2 crossScale;
    crossScale.x = crossSize.x / screenSize.x;
    crossScale.y = crossSize.y / screenSize.y;

    vec2 crossOffset = 0.5 - (crossScale * 0.5);
    vec2 crossMax = crossOffset + crossScale;
    vec2 crossUV = (texCoord - 0.5- (crossScale * 0.5))  / crossScale;

    vec4 crosshair = texture(crosshairTexture, crossUV);

    if (texCoord.x >= crossOffset.x && texCoord.y >= crossOffset.y
    && texCoord.x <= crossMax.x && texCoord.y <= crossMax.y) {
        color = mix(color, crosshair.rgb, crosshair.a);
    }

    return color;
}