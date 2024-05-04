struct LightStructure {
    vec3 position;
    float intensity;

    vec3 color;
    uint doesCastShadows;

    vec3 direction;
    uint isPointSource;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    uint shadowAtlasIndex;
    
    mat4 projectionViewMatrix;
};
