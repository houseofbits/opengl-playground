#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=80) out;

#define MAX_LIGHTS_PER_PASS 16

#include include/lightBlock.glsl

layout (std430, binding = 3) readonly buffer LightIndexesBlock {
    uint lightIndexes[MAX_LIGHTS_PER_PASS];
};

uniform uint numLightsPerPass;

in vec4 vsPosition[];
in vec3 vsNormal[];

out vec3 gsNormal;
out vec3 gsPosition;
out float gsFarPlane;

void main()
{   
    mat4 projViewMatrix;
    
    int viewportIndex=0;
    for(int i = 0; i < numLightsPerPass; i++)
    {
        uint lightIndex = lightIndexes[i];
        LightStructure light = lights[lightIndex];

        if (light.doesCastShadows == 1)
        {
            projViewMatrix = light.projectionViewMatrix;

            gl_ViewportIndex = viewportIndex;

            for(int a = 0; a < 3; ++a)
            {   
                vec3 ldir = normalize(light.position - vsPosition[a].xyz);
                gsNormal = vsNormal[a];
                float ndotl = 1.0 - dot(ldir, gsNormal);
                ldir = ldir * (ndotl  * 0.5);
                vec4 vpos = vec4(vsPosition[a].xyz - ldir, vsPosition[a].w); // - (gsNormal * 2.0),
                gl_Position = projViewMatrix * vpos;    //vsPosition[i];
                gsPosition = gl_Position.xyz;
                gsFarPlane = light.distAttenMax;

                EmitVertex();
            }    
            EndPrimitive();

            viewportIndex++;
        }
    }    
} 