#version 450

layout (location = 0) out vec4 fragColor;

in vec4 vsPosition;
in vec3 vsNormal;
in vec2 vsTexcoord;
in mat4 vsProjectionViewMatrix;

//float linearizeDepth (float depth) {
//    float nearPlane = 0.1, farPlane = 1000.0;
//    return (2.0 * nearPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
//}

void main()
{
    //    float depth = linearizeDepth(gl_FragCoord.z);
    //    depth = (depth - nearPlane)/(farPlane - nearPlane);

    vec2 moments;
    moments.x = gl_FragCoord.z;
    moments.y = gl_FragCoord.z * gl_FragCoord.z;

    float dx = dFdx( gl_FragCoord.z );
    float dy = dFdy( gl_FragCoord.z );
    moments.y += 0.25f * (dx * dx + dy * dy);

    vec4 lightSpacePosition = vsProjectionViewMatrix * vsPosition;

    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    float c = 99;
    float expDepth = exp(c * gl_FragCoord.z);
    //compare
//    float receiverDepth = exp(c * projCoords.z);
//    float shadowDepth = texture(shadowMap, projCoords.xy).r;
//    return clamp(shadowDepth / receiverDepth, 0.0, 1.0);

    fragColor = vec4(moments.x, moments.y, expDepth, 1.0);

//    fragColor = vec4(vec3(moments, projCoords.z), 1.0);
}
