#define LIGHTS_UNIFORM_BINDING_INDEX ${LIGHTS_UNIFORM_BINDING_INDEX}
#define MAX_LIGHTS ${MAX_LIGHTS}

layout (std140, binding = LIGHTS_UNIFORM_BINDING_INDEX) uniform LightBlock {
    Light lights[MAX_LIGHTS];
};

uniform uint numActiveLights;