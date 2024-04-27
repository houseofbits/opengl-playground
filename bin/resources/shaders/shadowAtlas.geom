#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=60) out;

#include include/structures.glsl
#include include/lightBlock.glsl

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
                gsNormal = vsNormal[i];
                gl_Position = projViewMatrix * vsPosition[i];
                gsPosition = gl_Position.xyz;
                gsFarPlane = lights[lightIndex].distAttenMax;

                EmitVertex();
            }    
            EndPrimitive();

            viewportIndex++;
        }
    }    
} 