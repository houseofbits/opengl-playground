#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    vec4 value = texture(texture1, TexCoord);
    // float nearZ = 0.01;
    // float farZ = 600;

    // float ndc_depth = value.z / value.w;
    // float depth = (((farZ - nearZ) * ndc_depth) + nearZ + farZ) / 2.0;

    // float ndcDepth = (2.0 * depth - 0.01 - 500) / 500;
    // float clipDepth = ndcDepth / depth;

    // FragColor = vec4((clipDepth * 0.5) + 0.5); 
    
    float color = pow(pow(value.z, 50), 100);

    FragColor = vec4(vec3(color), 1.0);
}