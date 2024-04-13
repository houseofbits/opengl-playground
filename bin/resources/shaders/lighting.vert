#version 410

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec3 Normal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{   
    vec4 p = modelMatrix * vec4(VertexPosition, 1.0);
    // vec4 n = modelMatrix * vec4(VertexNormal, 1.0);

    vs_out.FragPos = p.xyz;
    vs_out.Normal = VertexNormal;
    vs_out.TexCoord = TexCoord;
    
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(VertexPosition,1.0);
}
