#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#define MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS 6

struct ShadowAtlasFragment {    
    mat4 lightProjectionViewMatrix;
    vec4 viewport;
    vec4 lightPosition;
};

layout (std140, binding = 2) uniform ShadowAtlas {
    ShadowAtlasFragment shadowFragments[MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS];
};

uniform uint numberOfFragments;

in vec4 vsPosition[];
in vec3 vsNormal[];

out vec3 gsNormal;
out float gsDepth;
out float gsDistanceToLight;

void main()
{   
    mat4 projViewMatrix;
    vec4 lightPosition;

    for(int viewportIndex = 0; viewportIndex < numberOfFragments; viewportIndex++) 
    {
        projViewMatrix = shadowFragments[viewportIndex].lightProjectionViewMatrix;
        lightPosition = shadowFragments[viewportIndex].lightPosition;

        gl_ViewportIndex = viewportIndex;

        for(int i = 0; i < 3; ++i)
        {
            gsNormal = vsNormal[i];
            gl_Position = projViewMatrix * vsPosition[i];
            
            gsDepth = gl_Position.w;
            gsDistanceToLight = length(lightPosition - vsPosition[i]);

            EmitVertex();
        }    
        EndPrimitive();
    }
} 