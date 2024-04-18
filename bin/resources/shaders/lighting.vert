#version 450

#define LIGHTS_UNIFORM_BINDING_INDEX 1
#define LIGHT_VIEWS_UNIFORM_BINDING_INDEX 2

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TexCoord;

//---------------------------------------------------------
// Light views data
#define MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS 6   //This should be different than in a shadow pass

struct ShadowAtlasFragment {    
    mat4 lightProjectionViewMatrix;
    vec4 viewport;
    vec4 atlasRect;
};

layout (std140, binding = LIGHT_VIEWS_UNIFORM_BINDING_INDEX) uniform ShadowAtlas {
    ShadowAtlasFragment shadowFragments[MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS];
};

uniform uint numberOfFragments;

//---------------------------------------------------------
// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 lightViewMatrix;

//---------------------------------------------------------
// Output
out vec3 Normal;
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} vs_out;

out vec4 fragmentPositionPerLightView[MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS];

void main()
{   
    vec4 p = modelMatrix * vec4(VertexPosition, 1.0);
    // vec4 n = modelMatrix * vec4(VertexNormal, 1.0);

    vs_out.FragPos = p.xyz;
    vs_out.Normal = VertexNormal;
    vs_out.TexCoord = TexCoord;
    vs_out.FragPosLightSpace = lightViewMatrix * vec4(vs_out.FragPos, 1.0);
    
    for(int i = 0; i < numberOfFragments; i++) {
        fragmentPositionPerLightView[i] = shadowFragments[i].lightProjectionViewMatrix * vec4(vs_out.FragPos, 1.0);
    }

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(VertexPosition,1.0);
}
