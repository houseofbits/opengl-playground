#include "MeshResource.h"
#include <string>
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../../../Renderer/Model/ModelGLTFLoader.h"
#include "../../../Renderer/Model/GLTFFileLoader.h"
#include "../../../Renderer/Model/RenderModelBuilder.h"
#include <filesystem>

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

/**
* Note: Compile tinygltf with TINYGLTF_NO_EXTERNAL_IMAGE
*/

MeshResource::MeshResource() = default;

Resource::Status MeshResource::fetchData(ResourceManager &resourceManager) {
    auto model = GLTFFileLoader::loadModel(m_Path);
    if (model == nullptr) {
        return STATUS_FETCH_ERROR;
    }

    preloadMaterials(*model, resourceManager);

    ModelConfigurationLoader::createFromGLTFModel(*model, m_modelConfig, {true, true});

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
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, (void *) (mesh.offset * sizeof(GLuint)));
    }

    glBindVertexArray(0);
}

void MeshResource::render(const glm::mat4 &worldTransform, ShaderProgramResource &shader,
                          MaterialResource &defaultMaterial) {
    glBindVertexArray(m_model.m_vertexArrayObject);

    for (const auto &mesh: m_model.m_meshNodes) {
        if (mesh.materialIndex >= 0 && m_materials[mesh.materialIndex].isReady()) {
            m_materials[mesh.materialIndex].get().bind(shader);
        } else {
            defaultMaterial.bind(shader);
        }

        auto m = mesh.modelMatrix * worldTransform;
        shader.setUniform("modelMatrix", m);

        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT,
                       reinterpret_cast<void *>(mesh.offset * sizeof(GLuint)));
    }

    glBindVertexArray(0);
}

void MeshResource::render(const glm::mat4 &worldTransform, ShaderProgramResource &shader, bool overrideMaterial,
                          MaterialResource &defaultMaterial) {
    glBindVertexArray(m_model.m_vertexArrayObject);

    if (overrideMaterial) {
        glm::mat4 m;
        for (const auto &mesh: m_model.m_meshNodes) {
            m = mesh.modelMatrix * worldTransform;
            shader.setUniform("modelMatrix", m);

            glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT,
                           reinterpret_cast<void *>(mesh.offset * sizeof(GLuint)));
        }
    } else {
        for (const auto &mesh: m_model.m_meshNodes) {
            if (mesh.materialIndex >= 0 && m_materials[mesh.materialIndex].isReady()) {
                m_materials[mesh.materialIndex].get().bind(shader);
            } else {
                defaultMaterial.bind(shader);
            }

            auto m = mesh.modelMatrix * worldTransform;
            shader.setUniform("modelMatrix", m);

            glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT,
                           reinterpret_cast<void *>(mesh.offset * sizeof(GLuint)));
        }
    }

    glBindVertexArray(0);
}

void MeshResource::preloadMaterials(tinygltf::Model &model, ResourceManager &resourceManager) {
    std::filesystem::path pathObj(m_Path);
    std::filesystem::path dirPath = pathObj.parent_path();

    m_materials.reserve(model.materials.size());
    for (const auto &material: model.materials) {
        auto &materialResource = m_materials.emplace_back();
        // resourceManager.requestWith(materialResource, material.name,
        //                             [&](MaterialResource &resource) {
        //                                 resource.fetchFromGLTF(resourceManager, model, material, dirPath.string());
        //                             });
        resourceManager.request(materialResource, material.name);
    }
}

std::string MeshResource::getNodeMaterial(const std::string &nodeName) {
    for (const auto &node: m_model.m_meshNodes) {
        if (node.name == nodeName) {
            if (node.materialIndex >= 0 && node.materialIndex < m_materials.size()) {
                return m_materials[node.materialIndex].get().m_Path;
            }
        }
    }

    return "";
}
