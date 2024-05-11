#version 450

layout (location=0) out vec4 fragColor;

in vec3 gsNormal;
in vec3 gsPosition;
in vec2 gsTexcoord;
in mat3 gsInvTBN;

uniform vec3 viewPosition;
uniform uint diffuseTextureId;
uniform uint normalTextureId;

#include ../../shaders/include/textureAtlas.glsl

void main()
{
    vec2 uv = calculateAtlasUV(diffuseTextureId, gsTexcoord);
    vec3 diffuseColor = texture(diffuseAtlas, uv).xyz;

    vec2 uv2 = calculateAtlasUV(normalTextureId, gsTexcoord);
    vec3 normal = texture(normalsAtlas, uv2).rgb;
    normal = gsInvTBN * normalize(normal * 2.0 - 1.0);

    //    LightStructure light = lights[0];
    vec3 toLight = vec3(50, 100, 30) - gsPosition;
    vec3 lightDir = normalize(toLight);
    float ndotl = max(dot(lightDir, normal), 0);

    fragColor = vec4(vec3(diffuseColor * ndotl), 1.0);
}
