#define LIGHT_VIEWS_UNIFORM_BINDING_INDEX ${LIGHT_VIEWS_UNIFORM_BINDING_INDEX}
#define MAX_LIGHTVIEWS_PER_PASS 6

layout (std140, binding = LIGHT_VIEWS_UNIFORM_BINDING_INDEX) uniform LightViewsBlock {
    LightViewStructure lightViews[MAX_LIGHTVIEWS_PER_PASS];
};

uniform uint numberOfLightViews;
