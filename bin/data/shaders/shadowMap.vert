#version 450

struct SpotLightStructure {
    vec3 color;
    float intensity;
    vec3 position;
    float attenuation;
    mat4 projectionViewMatrix;
    vec3 direction;
    int isPointSource;
    uvec2 projectorSamplerHandle;
    uvec2 _PLACEHOLDER1;
    uvec2 shadowSamplerHandle;
    float bias;
    float blurRadius;
};

layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};
layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;

uniform uint SpotLightStorageBuffer_size;
uniform mat4 modelMatrix;
uniform int lightIndex;

//out vec4 vsPosition;
//out vec3 vsNormal;
//out vec2 vsTexcoord;
//out mat4 vsProjectionViewMatrix;

void main()
{
    mat4 viewProjectionMatrix = spotLights[lightIndex].projectionViewMatrix;

    mat3 modelInverse = mat3(transpose(inverse(modelMatrix)));

//    vsNormal = modelInverse * vertexNormal;
//    vsTexcoord = texCoord;
    vec4 vsPosition = modelMatrix * vec4(vertexPosition,1.0);
//    vsProjectionViewMatrix = spotLights[lightIndex].projectionViewMatrix;

    gl_Position = viewProjectionMatrix * vsPosition;
}
