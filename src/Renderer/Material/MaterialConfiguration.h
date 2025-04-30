#pragma once

#include <glm/vec3.hpp>
#include <string>

class MaterialConfiguration {
public:
    MaterialConfiguration() = default;

    enum TextureWrappingType {
        WRAPPING_UV0 = 0,
        WRAPPING_TRIPLANAR = 1,
    };

    glm::vec3 diffuseColor{1, 1, 1};
    glm::vec3 emissiveColor{0, 0, 0};
    std::string diffuseTextureUri;
    std::string normalTextureUri;
    std::string roughnessMetallicTextureUri;
    std::string emissiveTextureUri;
    float selfIllumination{0};
    TextureWrappingType diffuseTextureWrappingType{WRAPPING_UV0};
    TextureWrappingType normalTextureWrappingType{WRAPPING_UV0};
    TextureWrappingType roughnessMetallicTextureWrappingType{WRAPPING_UV0};
    bool doesCastShadows{true};
    bool doesReceiveShadows{true};
};
