#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/VertexArray.h"
#include "../../../Renderer/Model/Model.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include "MaterialResource.h"
#include "ShaderProgramResource.h"
#include "../../../Renderer/Model/ModelConfigurationLoader.h"

class MeshResource : public Resource {
public:
    MeshResource();

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override {
        //TODO Unload the mesh resource
    }

    void render(const glm::mat4 &worldTransform, ShaderProgramResource &, MaterialResource &);

    void render(ShaderProgramResource &);

    void render(const glm::mat4 &worldTransform, ShaderProgramResource &, bool overrideMaterial, MaterialResource &);

    std::string getNodeMaterial(const std::string &nodeName);

    std::vector<ResourceHandle<MaterialResource> > m_materials;
    ModelConfigurationLoader::ModelConfiguration m_modelConfig;

    Model m_model;

private:
    void preloadMaterials(tinygltf::Model &model, ResourceManager &resourceManager);


};
