
// Classic GLSL noise (by IQ)
float hash(vec3 p) {
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
    dot(p, vec3(269.5, 183.3, 246.1)),
    dot(p, vec3(113.5, 271.9, 124.6)));
    return fract(sin(p.x + p.y + p.z) * 43758.5453);
}

float noise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec3(1, 0, 0));
    float c = hash(i + vec3(0, 1, 0));
    float d = hash(i + vec3(1, 1, 0));
    float e = hash(i + vec3(0, 0, 1));
    float f1 = hash(i + vec3(1, 0, 1));
    float g = hash(i + vec3(0, 1, 1));
    float h = hash(i + vec3(1, 1, 1));

    vec3 u = f * f * (3.0 - 2.0 * f); // Smoothstep

    return mix(mix(mix(a, b, u.x),
                   mix(c, d, u.x), u.y),
               mix(mix(e, f1, u.x),
                   mix(g, h, u.x), u.y), u.z);
}

float hgPhase(float cosTheta, float g) {
    float g2 = g * g;
    return (1.0 - g2) / pow(1.0 + g2 - 2.0 * g * cosTheta, 1.5);
}

vec3 computeScatteredFogColor(vec3 samplePos, vec3 lightDir, vec3 viewDir, float height, vec3 mieColor, float rayleighFactor) {
    // Parameters
    float g = 0.8; // Mie scattering
    vec3 rayleighColor = vec3(0.6, 0.6, 0.8) * rayleighFactor;

    // View-light angle
    float cosTheta = dot(lightDir, viewDir);

    // Phase functions
    float miePhase = hgPhase(cosTheta, g);
    float rayleighPhase = 1.0 + cosTheta * cosTheta;

    // Altitude tint (less blue at low altitudes)
    float altitudeFactor = clamp(exp(-height * 0.1), 0.0, 1.0);

    // Final fog color (combine both scattering types)
    vec3 fogColor = rayleighColor * rayleighPhase * altitudeFactor +
    mieColor * miePhase * (1.0 - altitudeFactor);

    // Optional: scale intensity
    return fogColor * 0.5;
}

float getShadow(in sampler2DShadow shadowMap, mat4 lightMatrix, vec3 worldPos, float bias) {
    vec4 lightSpacePos = lightMatrix * vec4(worldPos, 1.0);
    vec3 lightProjectedPosition = calculateProjectedCoords(lightSpacePos);

    if (isProjCoordsClipping(lightProjectedPosition.xy) && lightProjectedPosition.z > 0.0 && lightProjectedPosition.z < 1.0) {

        return sampleShadow(shadowMap, lightProjectedPosition.xy, bias, lightProjectedPosition.z);
    }

    return 1.0;
}

vec3 calculateFog(SpotLightStructure light, vec3 viewPos, vec3 viewDir, float distToFragment, vec3 lightDir) {
    const int steps = 64;
    const float fogDensity = 0.03;
    const float stepSize = 100.0 / float(steps);

    vec3 fogColorAccum = vec3(0.0);

    int i = 0;
    for (float t = 0; t < distToFragment; t+=stepSize) {
        if (i > steps) {
            break;
        }

        vec3 samplePos = viewPos + viewDir * t;

        float shadow = 1.0;
        if (isSamplerHandleValid(light.shadowSamplerHandle)) {
            sampler2DShadow shadowMap = sampler2DShadow(light.shadowSamplerHandle);
            shadow = getShadow(shadowMap, light.projectionViewMatrix, samplePos, light.bias);
        }

        vec3 fogCol = computeScatteredFogColor(samplePos, lightDir, viewDir, samplePos.y, light.mieColor, light.rayleightFactor);

        fogColorAccum += fogDensity * fogCol * shadow;

        i++;
    }

    return clamp(fogColorAccum, 0.0, 1.0);
}
