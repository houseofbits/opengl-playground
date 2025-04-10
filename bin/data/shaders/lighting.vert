#version 450

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 texCoord;
layout (location=3) in vec3 tangent;
layout (location=4) in vec3 biTangent;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec4 vsPosition;
out vec3 vsNormal;
out vec2 vsTexcoord;
out mat3 vsInvTBN;

void main()
{
    mat3 modelInverse = mat3(transpose(inverse(modelMatrix)));

    vsNormal = modelInverse * vertexNormal;
    vsTexcoord = texCoord;
    vsPosition = modelMatrix * vec4(vertexPosition,1.0);

    vec3 T = normalize(vec3(modelMatrix * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(modelMatrix * vec4(-biTangent, 0.0)));
    vec3 N = normalize(vec3(modelMatrix * vec4(vertexNormal,    0.0)));

    vsInvTBN = mat3(T, B, N);

    gl_Position = viewProjectionMatrix * vsPosition;
}
