#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D image;

uniform vec2 blurDirection;

const int SAMPLE_COUNT = 13;

const float OFFSETS[13] = float[13](
    -6,
    -5,
    -4,
    -3,
    -2,
    -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6
);

const float WEIGHTS[13] = float[13](
    0.018815730430644367,
    0.03447396964662017,
    0.05657737457255749,
    0.08317258170844943,
    0.10952340502389689,
    0.12918787500405665,
    0.13649812722755,
    0.12918787500405665,
    0.10952340502389689,
    0.08317258170844943,
    0.05657737457255749,
    0.03447396964662017,
    0.018815730430644367
);


void main() {
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec2 offset = blurDirection * OFFSETS[i];
        float weight = WEIGHTS[i];
        result += imageLoad(image, ivec2(pixelCoord + offset)) * weight;
    }

    imageStore(image, pixelCoord, result);
}