#version 450

layout (location=0) out vec4 fragColor;

in vec3 gsNormal;
in float gsDepth;
in float gsDistanceToLight;

void main(){
    fragColor = vec4(vec3(gsDistanceToLight / 600), 1.0);
}
