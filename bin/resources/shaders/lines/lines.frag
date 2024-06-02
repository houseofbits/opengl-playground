#version 410

layout (location=0) out vec4 FragColor;

in vec4 gsColor;

void main() {   
    FragColor = gsColor;
}