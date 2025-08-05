
struct MaterialStructure {
    vec4 diffuseColor;
    vec3 emissiveColor;
    float selfIllumination;
    uvec2 diffuseTextureHandle;
    uvec2 normalTextureHandle;
    uvec2 roughnessTextureHandle;
    uvec2 emissiveTextureHandle;
    float roughnessFactor;
    float metallicFactor;
    int doesCastShadows;
    int doesReceiveShadows;
};

layout (binding = ${INDEX_MaterialsStorageBuffer}, std430) readonly buffer MaterialsStorageBuffer {
    MaterialStructure materials[100];
};

uniform uint MaterialsStorageBuffer_size;

vec4 getPrimaryDiffuseColor(MaterialStructure material, vec2 uv) {
    if (material.diffuseTextureHandle.x != 0 || material.diffuseTextureHandle.y != 0) {
        return texture(sampler2D(material.diffuseTextureHandle), uv);
    }

    return material.diffuseColor;
}

vec3 getPrimaryNormal(MaterialStructure material, vec2 uv, mat3 invTBN, vec3 surfaceNormal) {
    if (material.normalTextureHandle.x != 0 || material.normalTextureHandle.y != 0) {
        vec3 n = texture(sampler2D(material.normalTextureHandle), uv).xyz;

        return  invTBN * normalize(n * 2.0 - 1.0);
    }

    return surfaceNormal;
}

vec3 getPrimaryEmissiveFactor(MaterialStructure material, vec2 uv) {
    if (material.emissiveTextureHandle.x != 0 || material.emissiveTextureHandle.y != 0) {
        return texture(sampler2D(material.emissiveTextureHandle), uv).xyz;
    }

    return material.emissiveColor;
}

vec3 getPrimaryMROFactor(MaterialStructure material, vec2 uv) {
    if (material.roughnessTextureHandle.x != 0 || material.roughnessTextureHandle.y != 0) {
        return texture(sampler2D(material.roughnessTextureHandle), uv).xyz;
    }

    return vec3(material.metallicFactor, material.roughnessFactor, 1.0);
}