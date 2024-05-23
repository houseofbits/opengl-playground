#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=80) out;

#define MAX_LIGHTS_PER_PASS 16

#include include/structures.glsl
#include include/lightBlock.glsl

//layout (std140, binding = 2) uniform LightIndexesBlock {
//    uint lightIndexes[MAX_LIGHTS_PER_PASS];
//};
//
//uniform uint numLightsPerPass;

in vec4 vsPosition[];
in vec3 vsNormal[];

out vec3 gsNormal;
out vec3 gsPosition;
out float gsFarPlane;

void main()
{   
    mat4 projViewMatrix;
    
    int viewportIndex=0;
    for(int lightIndex = 0; lightIndex < numActiveLights; lightIndex++)
    {   
        if (lights[lightIndex].doesCastShadows == 1) 
        {
            projViewMatrix = lights[lightIndex].projectionViewMatrix;

            gl_ViewportIndex = viewportIndex;

            for(int i = 0; i < 3; ++i)
            {   
                vec3 ldir = normalize(lights[lightIndex].position - vsPosition[i].xyz);
                gsNormal = vsNormal[i];
                float ndotl = 1.0 - dot(ldir, gsNormal);
                ldir = ldir * (ndotl  * 1.2);
                vec4 vpos = vec4(vsPosition[i].xyz - ldir, vsPosition[i].w); // - (gsNormal * 2.0),
                gl_Position = projViewMatrix * vpos;    //vsPosition[i];
                gsPosition = gl_Position.xyz;
                gsFarPlane = lights[lightIndex].distAttenMax;

                EmitVertex();
            }    
            EndPrimitive();

            viewportIndex++;
        }
    }    
} 