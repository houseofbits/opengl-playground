float FXAA_SPAN_MAX = 32;
float FXAA_REDUCE_MIN = 1.0 / 256.0;
float FXAA_REDUCE_MUL = 1.0 / 8.0;

vec3 applyFXAA(in sampler2D textureIn, vec2 texCoord, vec2 inverseScreenSize) {
    vec3 rgbNW = texture(textureIn, texCoord + (vec2(- 1.0, - 1.0) * inverseScreenSize)).rgb;
    vec3 rgbNE = texture(textureIn, texCoord + (vec2(1.0, -1.0) * inverseScreenSize)).rgb;
    vec3 rgbSW = texture(textureIn, texCoord + (vec2(- 1.0, 1.0) * inverseScreenSize)).rgb;
    vec3 rgbSE = texture(textureIn, texCoord + (vec2(1.0, 1.0) * inverseScreenSize)).rgb;
    vec3 rgbM = texture(textureIn, texCoord).rgb;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = - ((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = clamp(dir * rcpDirMin, vec2(- FXAA_SPAN_MAX), vec2(FXAA_SPAN_MAX)) * inverseScreenSize;

    vec3 rgbA = 0.5 * (
    texture(textureIn, texCoord + dir * (1.0 / 3.0 - 0.5)).rgb +
    texture(textureIn, texCoord + dir * (2.0 / 3.0 - 0.5)).rgb);

    vec3 rgbB = rgbA * 0.5 + 0.25 * (
    texture(textureIn, texCoord + dir * - 0.5).rgb +
    texture(textureIn, texCoord + dir * 0.5).rgb);

    float lumaB = dot(rgbB, luma);

    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        return rgbA;
    }

    return rgbB;
}