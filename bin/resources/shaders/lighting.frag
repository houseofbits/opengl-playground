#version 450

layout (location=0) out vec4 FragColor;

#include include/structures.glsl
#include include/lightBlock.glsl
#include include/shadowAtlas.glsl

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;
uniform sampler2D texture1;

in vec4 fragmentPositionPerLightView[MAX_LIGHTS];

vec3 diffuseComponent(vec3 lightDir, vec3 normal, vec3 color)
{
    float ndotl = max(dot(lightDir, normal), 0.0);
    return ndotl * color;
}

vec3 specularComponent(vec3 lightDir, vec3 normal)
{
    vec3 viewDir = normalize(viewPosition - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
 
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    return vec3(1.0) * spec;
}

vec3 calculateSpotLightShadow(LightStructure light, vec3 projCoords, vec4 atlasRect)
{
    vec3 toLight = light.position - fs_in.FragPos;
    float dist = length(toLight);

    if (dist > light.distAttenMax) {
        return vec3(0.0);
    }    

    vec3 lightDir = normalize(toLight);
    vec3 normal = normalize(fs_in.Normal);
    float ndotl = dot(lightDir, normal);
    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

    float shadow = pcfShadowCalculation(projCoords, ndotl, atlasRect);
    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

    // float angle = acos(dot(-lightDir, light.direction));
    // float halfBeamAngle = light.beamAngle * 0.5;

    // if (angle > halfBeamAngle) {
    //     return vec3(0.0);
    // }

    float falloff = 1.0;//(halfBeamAngle - angle) / halfBeamAngle;
    float attenuation = 1.0 - clamp( dist / light.distAttenMax, 0.0, 1.0); 
    vec3 diffuse =  max(ndotl, 0.0) * light.color;
    vec3 specular = specularComponent(lightDir, normal);

    return shadow * light.intensity * falloff * attenuation * (diffuse + specular); 
}

void main() 
{
    // vec3 textureColor = texture(texture1, fs_in.TexCoord).xyz;

    // vec3 lightColor = vec3(0.0);
    // LightStructure light;
    // vec4 fragPosLightSpace;
 
    // for(int index = 0; index < numActiveLights; index++)
    // {   
    //     fragPosLightSpace = fragmentPositionPerLightView[index];
    //     light = lights[index];

    //     vec4 atlasRect = shadowAtlasRegions[light.shadowAtlasIndex];
    //     vec3 projCoords = getProjectedCoords(atlasRect, fragPosLightSpace);
    //     if (!projCoordsClip(atlasRect, projCoords)) {
    //         continue;
    //     }

    //     lightColor += calculateSpotLightShadow(light, projCoords, atlasRect);
    // }
    // FragColor = vec4(textureColor * lightColor, 1.0);

    // FragColor = vec4(fs_in.Normal, 1.0);


    // vec4 val = shadowAtlasRegions[0];
    // FragColor = vec4(val.x, val.y, 1.0, 1.0);

    vec3 shadowMap = vec3(0.0);
    float inShadow = 0;
    uint lightIndex = 0;
    LightStructure light = lights[lightIndex];

    vec4 atlasRect = shadowAtlasRegions[light.shadowAtlasIndex];
    vec3 projCoords = getProjectedCoords(atlasRect, fragmentPositionPerLightView[lightIndex]);
    
  // if (projCoordsClip2(atlasRect, projCoords.xy)) {
        shadowMap = texture(shadowDepthAtlas, projCoords.xy).xyz;

        // inShadow = sampleShadow(projCoords, 0.0000001);

        // shadowMap = calculateSpotLightShadow(light, projCoords);
  // }
//     FragColor = vec4(shadowMap, 1.0);

    float depth = pow(pow(shadowMap.r, 100), 100);
    FragColor = vec4(vec3(depth), 1.0);
}