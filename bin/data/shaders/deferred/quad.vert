#version 330 core

const vec2 vertices[4] = vec2[](
    vec2(-1.0, -1.0),  // Bottom-left
    vec2(1.0, -1.0),   // Bottom-right
    vec2(-1.0,  1.0),  // Top-left
    vec2(1.0,  1.0)    // Top-right
);

out vec2 TexCoords;

void main() {
    vec2 pos = vertices[gl_VertexID];
    TexCoords = pos * 0.5 + 0.5; // Convert [-1,1] to [0,1] for texture lookup
    gl_Position = vec4(pos, 0.0, 1.0);
}