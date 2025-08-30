#pragma once

#include "../../../Core/API.h"
#include "../Resources/MeshResource.h"
#include "../Resources/MaterialResource.h"

class MeshComponent : public Component {
    TYPE_DEFINITION(MeshComponent);

    inline static const std::string MODEL_KEY = "model";
    inline static const std::string OVERRIDE_MATERIAL_KEY = "material";

public:
    MeshComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    void setNodeMaterial(const std::string& nodeName, const std::string& materialName);

    std::string getNodeMaterial(const std::string& nodeName);

    void render(const glm::mat4 &worldTransform, ShaderProgramResource &shader, MaterialResource &);

    ResourceHandle<MeshResource> m_Mesh;
    ResourceHandle<MaterialResource> m_Material;
    bool m_shouldOverrideMaterial;
};