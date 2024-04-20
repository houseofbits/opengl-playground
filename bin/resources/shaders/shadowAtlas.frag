#version 450

layout (location=0) out vec4 fragColor;

in vec3 gsNormal;

void main(){
    fragColor = vec4(gsNormal, 1.0);
}
