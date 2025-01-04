#pragma once

#include "../../../Core/API.h"
#include "../Resources/MeshResource.h"

class MeshComponent : public Component {
    TYPE_DEFINITION(MeshComponent);

    inline static const std::string MODEL_KEY = "model";

public:
    MeshComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    ResourceHandle<MeshResource> m_Mesh;
};