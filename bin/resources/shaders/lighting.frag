#version 450

layout (location=0) out vec4 FragColor;

in vec3 gsNormal;
in vec4 gsPosition;
in vec2 gsTexcoord;
in mat3 gsInvTBN;

uniform vec3 viewPosition;
uniform uint diffuseTextureId;
uniform uint normalTextureId;
uniform uint specularTextureId;

#include include/lightBlock.glsl
#include include/textureAtlas.glsl

vec3 calculateProjectedCoords(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    return projCoords;
}
//
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
//
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

vec3 calculateSpecularComponent(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 specularColor)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    return spec * specularColor;
}

float calculateLightDistanceAttenuation(float distAttenMax, float distToLight)
{
    return 1.0 - clamp(distToLight / distAttenMax, 0.0, 1.0);
}

void main()
{
    vec2 uv = calculateAtlasUVBleed(diffuseTextureId, gsTexcoord);
    vec3 diffuseColor = texture(diffuseAtlas, uv).xyz;

    vec3 specularColor = vec3(1.0);
    if (specularTextureId > 0) {
        vec2 uvSpec = calculateAtlasUV(specularTextureId, gsTexcoord);
        specularColor = texture(diffuseAtlas, uvSpec).xyz;
    }

    vec3 normal = normalize(gsNormal);
    if (normalTextureId > 0) {
        vec2 uv2 = calculateAtlasUVBleed(normalTextureId, gsTexcoord);
        normal = texture(normalsAtlas, uv2).rgb;
        normal = gsInvTBN * normalize(normal * 2.0 - 1.0);
    }

    vec3 viewDir = normalize(viewPosition - gsPosition.xyz);
    vec3 lightProjColor = vec3(1.0);
    vec3 lightColor = vec3(0.0);

    for (int lightIndex = 0; lightIndex < numActiveLights; lightIndex++) {
        LightStructure light = lights[lightIndex];

        vec4 lightSpacePosition = light.projectionViewMatrix * gsPosition;
        vec3 lightProjectedPosition = calculateProjectedCoords(lightSpacePosition);
        if (isProjCoordsClipping(lightProjectedPosition.xy) && lightProjectedPosition.z > 0.0 && lightProjectedPosition.z < 1.0) {
            vec3 toLight = light.position - gsPosition.xyz;
            vec3 lightDir = normalize(toLight);
            float distToLight = length(toLight);
            float ndotlSurf = dot(lightDir, gsNormal);
            float ndotl = dot(lightDir, normal);

            if (ndotlSurf < 0 || distToLight > light.distAttenMax) {
                continue;
            }

            float shadowing = 1.0;
            if (light.doesCastShadows == 1) {
                shadowing = pcfShadowCalculation(lightProjectedPosition, light.shadowAtlasIndex, ndotl);
            }

            lightProjColor = vec3(1.0);
            if (light.projectionTextureId != 0) {
                vec2 lightProjUV = calculateAtlasUV(light.projectionTextureId, lightProjectedPosition.xy);
                lightProjColor = texture(effectsAtlas, lightProjUV).xyz;
            }

            vec3 diffuse = diffuseColor
                * ndotl
                * light.color
                * light.intensity;

            lightColor +=
                calculateLightDistanceAttenuation(light.distAttenMax, distToLight)
                * shadowing
                * lightProjColor
                * (diffuse + calculateSpecularComponent(lightDir, viewDir, normal, specularColor));
        }
    }

    FragColor = vec4(lightColor, 1.0);
//    FragColor = vec4(vec3(normal), 1.0);
}