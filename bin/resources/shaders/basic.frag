#version 450

layout (location=0) out vec4 fragColor;
// layout (location = 0) out float fragmentDepth;

uniform vec3 viewPosition;
uniform vec4 modelColor;

in vec4 position;
in vec3 normal;
in vec3 uv;
in vec4 color;

vec3 sunPos = vec3(10, 10, 10);
vec3 lightColor = vec3(1,1,1);

void main()
{
    vec3 lightDir   = normalize(sunPos - position.xyz);
    vec3 viewDir    = normalize(viewPosition - position.xyz);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(lightDir, normal), 0.0);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = lightColor * spec;

    vec3 c = modelColor.xyz * color.xyz;

    fragColor = vec4((c * diff) + spec + vec3(0.1 * c), 1.0);
}
