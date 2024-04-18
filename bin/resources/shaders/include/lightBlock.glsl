#define LIGHTS_UNIFORM_BINDING_INDEX 1
#define MAX_LIGHTS 10

layout (std140, binding = LIGHTS_UNIFORM_BINDING_INDEX) uniform LightBlock {
    Light lights[MAX_LIGHTS];
};

uniform uint numActiveLights;