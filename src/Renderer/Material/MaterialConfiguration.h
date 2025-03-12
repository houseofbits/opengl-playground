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
    std::string diffuseTextureUri;
    std::string normalTextureUri;
    std::string roughnessTextureUri;
    float selfIllumination{0};
    TextureWrappingType diffuseTextureWrappingType{WRAPPING_UV0};
    TextureWrappingType normalTextureWrappingType{WRAPPING_UV0};
    TextureWrappingType roughnessTextureWrappingType{WRAPPING_UV0};
    bool doesCastShadows{true};
    bool doesReceiveShadows{true};
};
