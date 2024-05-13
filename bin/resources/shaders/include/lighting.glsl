
//vec3 specularComponent(vec3 lightDir, vec3 normal)
//{
//    vec3 viewDir = normalize(viewPosition - fs_in.FragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = 0.0;
//
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//
//    return vec3(1.0) * spec;
//}

vec3 calculateLight(LightStructure light, float ndotl, float distToLight)
{
    float attenuation = 1.0 - clamp( distToLight / light.distAttenMax, 0.0, 1.0);
    vec3 diffuse =  ndotl * light.color;
    vec3 specular = vec3(1.0);//specularComponent(lightDir, normal);

    return light.intensity * attenuation * (diffuse + specular);
}