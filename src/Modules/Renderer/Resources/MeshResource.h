#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/VertexArray.h"
#include "../../../Renderer/Model/Model.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include "MaterialResource.h"
#include "ShaderProgramResource.h"

class MeshResource : public Resource {
public:
    MeshResource();

    Status fetchData(ResourceManager &) override;

    Status build() override;

    void destroy() override {
        //TODO Unload the mesh resource
    }

    void render(ShaderProgramResource &, MaterialResource &);

private:
    void preloadMaterials(ResourceManager &resourceManager);

    std::vector<ResourceHandle<MaterialResource> > m_materials;
    tinygltf::Model *m_temporaryModel;
    Model m_model;
};
