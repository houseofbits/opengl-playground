#include "MeshResource.h"
#include <string>
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../../../Renderer/Model/ModelGLTFLoader.h"
#include "../../../Renderer/Model/GLTFFileLoader.h"

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

/**
* Note: Compile tinygltf with TINYGLTF_NO_EXTERNAL_IMAGE
*/

MeshResource::MeshResource(): m_temporaryModel(nullptr) {
}

Resource::Status MeshResource::fetchData(ResourceManager &resourceManager) {
    m_temporaryModel = GLTFFileLoader::loadModel(m_Path);
    if (m_temporaryModel == nullptr) {
        return STATUS_FETCH_ERROR;
    }

    preloadMaterials(resourceManager);

    return STATUS_DATA_READY;
}

Resource::Status MeshResource::build() {
    ModelGLTFLoader::createFromGLTFModel(*m_temporaryModel, m_model);

    delete m_temporaryModel;

    return STATUS_READY;
}

void MeshResource::render(ShaderProgramResource &shader, MaterialResource &defaultMaterial) {
    for (auto &mesh: m_model.m_meshes) {
        if (mesh->materialIndex >= 0 && m_materials[mesh->materialIndex].isReady()) {
            m_materials[mesh->materialIndex].get().bind(shader);
        } else {
            defaultMaterial.bind(shader);
        }

        shader.setUniform("modelMatrix", mesh->modelMatrix);
        for (const auto &element: mesh->vertexArray.elementsArray) {
            glBindVertexArray(element->vaoId);
            glDrawElements(element->mode, static_cast<GLsizei>(element->count),
                           element->componentType,
                           BUFFER_OFFSET(element->bufferOffset));
        }
    }
}

void MeshResource::preloadMaterials(ResourceManager &resourceManager) {
    m_materials.reserve(m_temporaryModel->materials.size());
    for (const auto &material: m_temporaryModel->materials) {
        auto &materialResource = m_materials.emplace_back();
        resourceManager.requestWith(materialResource, material.name,
                                    [&](MaterialResource &resource) {
                                        resource.fetchFromGLTF(resourceManager, *m_temporaryModel, material);
                                    });
    }
}
