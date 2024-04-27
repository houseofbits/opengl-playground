#version 450

// layout (location=0) out vec4 fragColor;
layout (location = 0) out float fragmentDepth;

in vec3 gsNormal;
in vec3 gsPosition;
in float gsFarPlane;

void main(){

    float l = length(gsPosition) / gsFarPlane;
    // // fragColor = vec4(gsNormal, 1.0);
    // fragColor = vec4(vec3(l), 1.0);
    fragmentDepth = l;
}
