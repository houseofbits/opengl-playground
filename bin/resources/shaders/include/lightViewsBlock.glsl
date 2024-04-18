#define LIGHT_VIEWS_UNIFORM_BINDING_INDEX ${LIGHT_VIEWS_UNIFORM_BINDING_INDEX}
#define MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS 6

layout (std140, binding = LIGHT_VIEWS_UNIFORM_BINDING_INDEX) uniform ShadowAtlas {
    ShadowAtlasFragment shadowFragments[MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS];
};

uniform uint numberOfFragments;