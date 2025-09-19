bool isSamplerHandleValid(uvec2 handle) {
    return (handle.x != 0 || handle.y != 0);
}

vec3 calculateProjectedCoords(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    return projCoords;
}

bool isProjCoordsClipping(vec2 uv)
{
    if (uv.x < 0
    || uv.x > 1.0
    || uv.y < 0
    || uv.y > 1.0) {

        return false;
    }

    return true;
}

float sampleShadow(in sampler2DShadow shadowMap, vec2 uv, float bias, float fragmentDepth)
{
    return texture(shadowMap, vec3(uv, fragmentDepth - bias)).x;
}
