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
uniform vec4 diffuseColor;
uniform float selfIllumination;
uniform bool doesReceiveShadows;
uniform uint specularPower;
uniform vec3 specularColor;

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
    float blurFactor = 1.0 / 800.0; //textureSize(shadowDepthAtlas, 0).x;  //
    float shadow = 0;
    vec2 uv;
    int div=0;
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
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

vec3 calculateSpecularComponent(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 color, uint power)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), float(power));

    return spec * color;
}

float calculateLightDistanceAttenuation(float distAttenMax, float distToLight)
{
    return 1.0 - clamp(distToLight / distAttenMax, 0.0, 1.0);
}

void main()
{
    vec3 diffuse = diffuseColor.rgb;
    if (diffuseTextureId > 0) {
        diffuse = sampleDiffuseAtlasFragment(diffuseTextureId, gsTexcoord);
        diffuse *= diffuseColor.rgb;
    }

    if (selfIllumination > 0.9) {
        FragColor = vec4(diffuse, 1.0);
        return;
    }

    vec3 specularLevel = vec3(1.0);
    if (specularTextureId > 0) {
        specularLevel = sampleDiffuseAtlasFragment(specularTextureId, gsTexcoord);
    }

    vec3 normal = gsNormal;
    if (normalTextureId > 0) {
        normal = sampleNormalsAtlasFragment(normalTextureId, gsTexcoord);
        normal = gsInvTBN * normalize(normal * 2.0 - 1.0);
    }
    normal = normalize(normal);

    vec3 viewDir = normalize(viewPosition - gsPosition.xyz);
    vec3 lightProjColor = vec3(1.0);
    vec3 lightColor = vec3(selfIllumination) + (diffuse * vec3(0.1,0.1,0.1));

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
            if (light.doesCastShadows == 1 && doesReceiveShadows) {
                shadowing = pcfShadowCalculation(lightProjectedPosition, light.shadowAtlasIndex, ndotl);
            }

            lightProjColor = vec3(1.0);
            if (light.projectionTextureId != 0) {
                vec2 lightProjUV = calculateAtlasUV(light.projectionTextureId, lightProjectedPosition.xy);
                lightProjColor = texture(effectsAtlas, lightProjUV).xyz;
            }

            vec3 diffuseLight = diffuse
                * ndotl
                * light.color
                * light.intensity;

            lightColor +=
                calculateLightDistanceAttenuation(light.distAttenMax, distToLight)
                * shadowing
                * lightProjColor
                * (diffuseLight + calculateSpecularComponent(lightDir, viewDir, normal, specularLevel * specularColor, specularPower));
        }
    }

    if (selfIllumination > 0) {
        lightColor = mix(lightColor, diffuse, selfIllumination);
    }

    FragColor = vec4(lightColor, 1.0);
}