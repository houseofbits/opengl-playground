#version 450

#extension GL_ARB_bindless_texture : require

#include include/commonBlock.glsl
#include include/lightsBlock.glsl
#include include/fogBlock.glsl

out vec4 FragColor;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;

layout(bindless_sampler) uniform samplerCube environmentSampler;
uniform vec3 viewPosition;

void main()
{
    vec3 viewDepth = vsPosition.xyz - viewPosition;
    vec3 view = normalize(viewDepth);
    float distToFragment = 100;//length(viewDepth);
    vec3 sky = texture(environmentSampler, view).rgb;

    vec4 fogColor = vec4(0.0);
    vec3 lightDir;

    for (int lightIndex = 0; lightIndex < SpotLightStorageBuffer_size; lightIndex++) {
        SpotLightStructure light = spotLights[lightIndex];
        vec4 lightSpacePosition = light.projectionViewMatrix * vsPosition;
        vec3 lightProjectedPosition = calculateProjectedCoords(lightSpacePosition);

        if (isProjCoordsClipping(lightProjectedPosition.xy) && lightProjectedPosition.z > 0.0 && lightProjectedPosition.z < 1.0) {
            if (light.isPointSource == 1) {
                vec3 toLight = light.position - vsPosition.xyz;
                lightDir = normalize(toLight);
            } else {
                lightDir = normalize(light.direction);
            }

            if (light.isAtmosphericEffectsEnabled == 1) {
                fogColor += calculateFog(light, viewPosition, view, distToFragment, lightDir);
            }
        }
    }

    vec3 finalColor = sky * (1.0 - fogColor.rgb) + fogColor.rgb;
//    vec3 finalColor = mix(sky, fogColor.rgb, fogColor.a);

    FragColor = vec4(finalColor, 1.0);

//    FragColor = vec4(sky, 1.0);
}