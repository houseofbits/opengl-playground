#define LIGHT_POINT_SOURCE = 1 << 0
#define LIGHT_DIRECT_SOURCE = 1 << 1
#define LIGHT_HAS_SHADOW = 1 << 2

struct LightStructure {
    vec3 position;
    float falloff;    

    vec3 color;
    float intensity;

    vec3 direction;
    uint flags;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;  

    vec2 shadowAtlasPos;
    vec2 shadowAtlasSize;
    mat4 projectionViewMatrix;
};

struct LightViewStructure {    
    mat4 lightProjectionViewMatrix;
    vec2 atlasPos;
    vec2 atlasSize;
    uint lightIndex;
};