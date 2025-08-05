struct SpotLightStructure {
    vec3 color;
    float intensity;
    vec3 position;
    float attenuation;
    mat4 projectionViewMatrix;
    vec3 direction;
    int isPointSource;
    uvec2 projectorSamplerHandle;
    float _PLACEHOLDER1;
    int isAtmosphericEffectsEnabled;
    uvec2 shadowSamplerHandle;
    float bias;
    float blurRadius;
    vec3 mieColor;
    float rayleightFactor;
};

layout (binding = ${INDEX_SpotLightStorageBuffer}, std430) readonly buffer SpotLightStorageBuffer {
    SpotLightStructure spotLights[100];
};

uniform uint SpotLightStorageBuffer_size;