#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=60) out;

uniform mat4 viewProjectionMatrix;

in vec4 vsPosition[];
in vec3 vsNormal[];
in vec2 vsTexcoord[];

out vec3 gsNormal;
out vec4 gsPosition;
out vec3 gsScreenPosition;
out vec2 gsTexcoord;
out mat3 gsInvTBN;

void main()
{
    vec3 edge1 = vsPosition[1].xyz - vsPosition[0].xyz;
    vec3 edge2 = vsPosition[2].xyz - vsPosition[0].xyz;
    vec2 uv1 = vsTexcoord[1] - vsTexcoord[0];
    vec2 uv2 = vsTexcoord[2] - vsTexcoord[0];

    float f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
    vec3 T = f * (uv2.y * edge1 - uv1.y * edge2);
    vec3 B = f * (-uv2.x * edge1 + uv1.x * edge2);

    T = normalize(T);
    B = normalize(-B);
    vec3 N = normalize(cross(edge1, edge2));

    gsInvTBN = inverse(transpose(mat3(T, B, N)));

    for (int i = 0; i < 3; ++i)
    {
        gsNormal = vsNormal[i];
        gsTexcoord = vsTexcoord[i];
        gsPosition = vsPosition[i];
        gl_Position = viewProjectionMatrix * vsPosition[i];
        gsScreenPosition = (gl_Position.xyz / gl_Position.w) * 0.5 + 0.5;;

        EmitVertex();
    }
    EndPrimitive();
}