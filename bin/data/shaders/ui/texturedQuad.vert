#version 330 core

const vec2 texCoords[4] = vec2[](
    vec2(0.0, 0.0), // Bottom-left
    vec2(1.0, 0.0), // Bottom-right
    vec2(0.0, 1.0), // Top-left
    vec2(1.0, 1.0)    // Top-right
);

out vec2 TexCoords;

uniform vec2 size;
uniform vec2 position;

vec2 vertices[4] = vec2[](
    position, // Bottom-left
    position + vec2(size.x, 0), // Bottom-right
    position + vec2(0, size.y), // Top-left
    position + size   // Top-right
);

void main() {
    vec2 pos = vertices[gl_VertexID];
    pos = pos * 2.0 - 1.0;

    TexCoords = texCoords[gl_VertexID];
    gl_Position = vec4(pos, 0.0, 1.0);
}