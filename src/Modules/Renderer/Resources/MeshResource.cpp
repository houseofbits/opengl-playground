#include "MeshResource.h"
#include <string>
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../../../Renderer/Model/ModelGLTFLoader.h"
#include "../../../Renderer/Model/GLTFFileLoader.h"
#include "../../../Renderer/Model/RenderModelBuilder.h"

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

/**
* Note: Compile tinygltf with TINYGLTF_NO_EXTERNAL_IMAGE
*/

MeshResource::MeshResource() {
}

Resource::Status MeshResource::fetchData(ResourceManager &resourceManager) {
    auto model = GLTFFileLoader::loadModel(m_Path);
    if (model == nullptr) {
        return STATUS_FETCH_ERROR;
    }

    preloadMaterials(*model, resourceManager);

    ModelConfigurationLoader::createFromGLTFModel(*model, m_modelConfig);

    delete model;

    return STATUS_DATA_READY;
}

Resource::Status MeshResource::build() {
    RenderModelBuilder::buildFromModelConfiguration(m_modelConfig, m_model);

    return STATUS_READY;
}

void MeshResource::render(ShaderProgramResource &shader) {
    glBindVertexArray(m_model.m_vertexArrayObject);

    for (auto &mesh: m_model.m_meshNodes) {
        shader.setUniform("modelMatrix", mesh.modelMatrix);
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, (void *) (mesh.offset * sizeof(GLuint)));
    }

    glBindVertexArray(0);
}

void MeshResource::render(ShaderProgramResource &shader, MaterialResource &defaultMaterial) {
    glBindVertexArray(m_model.m_vertexArrayObject);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model.m_elementBufferObject);

    for (auto &mesh: m_model.m_meshNodes) {
        if (mesh.materialIndex >= 0 && m_materials[mesh.materialIndex].isReady()) {
            m_materials[mesh.materialIndex].get().bind(shader);
        } else {
            defaultMaterial.bind(shader);
        }

        shader.setUniform("modelMatrix", mesh.modelMatrix);
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, (void *) (mesh.offset * sizeof(GLuint)));
    }

    glBindVertexArray(0);
}

void MeshResource::preloadMaterials(tinygltf::Model &model, ResourceManager &resourceManager) {
    m_materials.reserve(model.materials.size());
    for (const auto &material: model.materials) {
        auto &materialResource = m_materials.emplace_back();
        resourceManager.requestWith(materialResource, material.name,
                                    [&](MaterialResource &resource) {
                                        resource.fetchFromGLTF(resourceManager, model, material);
                                    });
    }
}
