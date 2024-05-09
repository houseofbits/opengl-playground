#version 450

layout (location=0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;
uniform uint diffuseTextureId;
//uniform sampler2D texture1;

#include include/structures.glsl
#include include/lightBlock.glsl
#include include/textureAtlas.glsl
#include include/lighting.glsl

in vec4 fragmentPositionPerLightView[MAX_LIGHTS];



vec3 calculateProjectedCoords(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    return projCoords;
}

float sampleShadow(vec3 projCoords, float bias)
{
    float depth = texture(shadowDepthAtlas, projCoords.xy).r;
    return projCoords.z - bias > depth ? 0.0 : 1.0;
}

float sampleShadow(uint shadowAtlasIndex, vec2 uv, float bias, float fragmentDepth)
{
    uv = calculateAtlasUV(shadowAtlasIndex, uv);

    float depth = texture(shadowDepthAtlas, uv).r;
    return ((fragmentDepth - bias) > depth) ? 0.0 : 1.0;
}

float pcfShadowCalculation(vec3 projCoords, uint shadowAtlasIndex, float ndotl)
{
    //    float currentDepth = projCoords.z;
    //    vec2 uv = calculateAtlasUV(shadowAtlasIndex, projCoords.xy);
    //    float dstToSurface = abs(texture(shadowDepthAtlas, uv).r - currentDepth);

    float bias = 0.0000001;
    float blurFactor = 1.0 / 256.0;//textureSize(shadowDepthAtlas, 0);
    float shadow = 0;
    vec2 uv;
    int div=0;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            uv = projCoords.xy + vec2(x, y) * blurFactor;
            if (isProjCoordsClipping(uv))
            {
                shadow += sampleShadow(shadowAtlasIndex, uv, bias, projCoords.z);

                div++;
            }
        }
    }
    shadow /= div;

    return shadow;
}

void main()
{
    vec2 uv = calculateAtlasUV(diffuseTextureId, fs_in.TexCoord);
    vec3 diffuseColor = texture(diffuseAtlas, uv).xyz;
    vec3 lightProjColor = vec3(1.0);
    vec3 lightColor = vec3(0.0);

    for (int lightIndex = 0; lightIndex < numActiveLights; lightIndex++) {
        LightStructure light = lights[lightIndex];

        vec3 projCoords = calculateProjectedCoords(fragmentPositionPerLightView[lightIndex]);
        if (isProjCoordsClipping(projCoords.xy) && projCoords.z > 0.0 && projCoords.z < 1.0) {
            vec3 lightPosition = light.position;
            vec3 toLight = lightPosition - fs_in.FragPos;
            vec3 lightDir = normalize(toLight);
            float ndotl = dot(lightDir, fs_in.Normal);

            if (ndotl < 0) {
                continue;
            }

            float shadowing = 1.0;
            if (light.doesCastShadows == 1) {
                shadowing = pcfShadowCalculation(projCoords, light.shadowAtlasIndex, ndotl);
            }

            lightProjColor = vec3(1.0);
            if (light.projectionTextureId != 0) {
                vec2 lightProjUV = calculateAtlasUV(light.projectionTextureId, projCoords.xy);
                lightProjColor = texture(effectsAtlas, lightProjUV).xyz;
            }

            lightColor += calculateLight(light, shadowing, lightProjColor);
        }
    }
    FragColor = vec4(diffuseColor * lightColor, 1.0);
}