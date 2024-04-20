#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#include include/structures.glsl
#include include/lightViewsBlock.glsl

in vec4 vsPosition[];
in vec3 vsNormal[];

out vec3 gsNormal;

void main()
{   
    mat4 projViewMatrix;

    for(int viewportIndex = 0; viewportIndex < numberOfLightViews; viewportIndex++) 
    {
        projViewMatrix = lightViews[viewportIndex].lightProjectionViewMatrix;

        gl_ViewportIndex = viewportIndex;

        for(int i = 0; i < 3; ++i)
        {
            gsNormal = vsNormal[i];
            gl_Position = projViewMatrix * vsPosition[i];

            EmitVertex();
        }    
        EndPrimitive();
    }
} 