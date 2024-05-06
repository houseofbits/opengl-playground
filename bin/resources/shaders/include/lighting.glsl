

vec3 diffuseComponent(vec3 lightDir, vec3 normal, vec3 color)
{
    float ndotl = max(dot(lightDir, normal), 0.0);
    return ndotl * color;
}

vec3 specularComponent(vec3 lightDir, vec3 normal)
{
    vec3 viewDir = normalize(viewPosition - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    return vec3(1.0) * spec;
}

vec3 calculateLight(LightStructure light, float shadow)
{
    vec3 toLight = light.position - fs_in.FragPos;
    float dist = length(toLight);

    if (dist > light.distAttenMax) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(toLight);
    vec3 normal = normalize(fs_in.Normal);
    float ndotl = dot(lightDir, normal);
    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

    if (ndotl <= 0.0) {
        return vec3(0.0);
    }

     float angle = acos(dot(-lightDir, light.direction));
     float halfBeamAngle = light.beamAngle * 0.5;

     if (angle > halfBeamAngle) {
         return vec3(0.0);
     }

    float falloff = (halfBeamAngle - angle) / halfBeamAngle;
    float attenuation = 1.0 - clamp( dist / light.distAttenMax, 0.0, 1.0);
    vec3 diffuse =  max(ndotl, 0.0) * light.color;
    vec3 specular = specularComponent(lightDir, normal);

    return shadow * light.intensity * falloff * attenuation * (diffuse + specular);
}