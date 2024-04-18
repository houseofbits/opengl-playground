
#define LIGHT_TYPE_POINT 0 
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_SPOT_WITH_SHADOW 4

struct Light {
    vec3 position;
    float falloff;    

    vec3 color;
    float intensity;

    vec3 direction;
    uint type;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;    
};

//Point light will have up to 6 views per light
struct ShadowAtlasFragment {    
    mat4 lightProjectionViewMatrix;
    vec4 viewport;
    vec4 atlasRect;
    // uint lightIndex;
};