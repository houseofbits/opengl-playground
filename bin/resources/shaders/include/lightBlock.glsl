#define LIGHTS_UNIFORM_BINDING_INDEX ${LIGHTS_UNIFORM_BINDING_INDEX}
#define MAX_LIGHTS ${MAX_LIGHTS}

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

    uint projectionTextureId;
};

layout (std140, binding = LIGHTS_UNIFORM_BINDING_INDEX) uniform LightBlock {
    LightStructure lights[MAX_LIGHTS];
};

uniform uint numActiveLights;