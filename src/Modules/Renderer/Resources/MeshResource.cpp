#include "MeshResource.h"
#include <fbxsdk.h>
#include <iostream>
#include <string>

MeshResource::MeshResource() {
}

Resource::Status MeshResource::build() {
    FbxScene *scene = loadFromFile();
    if (scene == nullptr) {
        return STATUS_BUILD_ERROR;
    }

    FbxGeometryConverter converter(scene->GetFbxManager());

    processMeshNode(scene->GetRootNode(), converter);

    scene->Destroy();

    return STATUS_READY;
}

FbxScene *MeshResource::loadFromFile() const {
    FbxManager *manager = nullptr;
    FbxScene *scene = nullptr;

    //Initialize manager and scene
    manager = FbxManager::Create();
    if (!manager) {
        Log::error(m_Path, " Error: Unable to create FBX Manager!");

        return nullptr;
    }

    FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    scene = FbxScene::Create(manager, "Scene");
    if (!scene) {
        Log::error(m_Path, " Error: Unable to create FBX Scene!");

        manager->Destroy();

        return nullptr;
    }

    //Import file
    FbxImporter *importer = FbxImporter::Create(manager, "");

    if (!importer->Initialize(m_Path.c_str(), -1, manager->GetIOSettings())) {
        Log::error(m_Path, " Error: Unable to initialize importer");
        Log::error(m_Path, " Error: ", importer->GetStatus().GetErrorString());

        importer->Destroy();
        manager->Destroy();

        return nullptr;
    }

    if (!importer->Import(scene)) {
        Log::error(m_Path, " Error: Unable to import FBX file!");

        importer->Destroy();
        manager->Destroy();

        return nullptr;
    }

    importer->Destroy();
    manager->Destroy();

    return scene;
}

void MeshResource::render() {
    for (auto &mesh: m_meshes) {
        mesh.vertexArray.draw();
    }
}

void MeshResource::processMeshNode(FbxNode *node, FbxGeometryConverter &converter) {
    FbxMesh *mesh = node->GetMesh();
    if (mesh != nullptr) {
        if (!mesh->IsTriangleMesh()) {
            std::cout << "Triangulating mesh: " << node->GetName() << std::endl;
            const auto attrib = converter.Triangulate(mesh, true);

            mesh = attrib->GetNode()->GetMesh();

            if (mesh->GenerateTangentsDataForAllUVSets()) {
                std::cout << "Tangents generated successfully for mesh: " << mesh->GetName() << std::endl;
            } else {
                std::cerr << "Failed to generate tangents for mesh: " << mesh->GetName() << std::endl;
            }
        }

        std::vector<unsigned int> indices;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<float> tangents;

        FbxAMatrix globalTransform = node->EvaluateGlobalTransform();

        getMeshAttributes(mesh, indices, vertices, normals, texCoords, tangents);

        auto &ref = m_meshes.emplace_back();
        ref.name = node->GetName();
        ref.modelMatrix = glm::mat4(1.0f);
        ref.vertexArray.createBuffers(&indices, &vertices, &normals, &texCoords, &tangents);
    }

    for (int i = 0; i < node->GetChildCount(); ++i) {
        processMeshNode(node->GetChild(i), converter);
    }
}

void MeshResource::getMeshAttributes(FbxMesh *mesh, std::vector<unsigned int> &indices, std::vector<float> &vertices,
                                     std::vector<float> &normals, std::vector<float> &texCoords,
                                     std::vector<float> &tangents) const {
    if (!mesh) return;

    const FbxVector4 *controlPoints = mesh->GetControlPoints();

    const FbxGeometryElementNormal *normalElement = mesh->GetElementNormal();
    const bool hasNormals = (normalElement != nullptr);

    const FbxGeometryElementUV *uvElement = mesh->GetElementUV();
    const bool hasUVs = (uvElement != nullptr);

    const FbxGeometryElementTangent *tangentElement = mesh->GetElementTangent();
    const bool hasTangents = (tangentElement != nullptr);

    int polygonCount = mesh->GetPolygonCount();
    int index = 0;
    for (int i = 0; i < polygonCount; ++i) {
        int polygonSize = mesh->GetPolygonSize(i);

        for (int j = 0; j < polygonSize; ++j) {
            int controlPointIndex = mesh->GetPolygonVertex(i, j);

            FbxVector4 pos = controlPoints[controlPointIndex];
            vertices.push_back(static_cast<float>(pos[0]));
            vertices.push_back(static_cast<float>(pos[1]));
            vertices.push_back(static_cast<float>(pos[2]));

            if (hasNormals) {
                FbxVector4 normal;
                mesh->GetPolygonVertexNormal(i, j, normal);
                normals.push_back(static_cast<float>(normal[0]));
                normals.push_back(static_cast<float>(normal[1]));
                normals.push_back(static_cast<float>(normal[2]));
            }

            if (hasUVs) {
                FbxVector2 uv;
                bool unmapped;
                mesh->GetPolygonVertexUV(i, j, uvElement->GetName(), uv, unmapped);
                texCoords.push_back(static_cast<float>(uv[0]));
                texCoords.push_back(static_cast<float>(uv[1]));
            }

            if (hasTangents) {
                FbxVector4 tangent;
                FbxGeometryElement::EMappingMode mappingMode = tangentElement->GetMappingMode();
                FbxGeometryElement::EReferenceMode referenceMode = tangentElement->GetReferenceMode();

                if (mappingMode == FbxGeometryElement::eByControlPoint) {
                    if (referenceMode == FbxGeometryElement::eDirect) {
                        tangent = tangentElement->GetDirectArray().GetAt(controlPointIndex);
                    } else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                        const int tangentIndex = tangentElement->GetIndexArray().GetAt(controlPointIndex);
                        tangent = tangentElement->GetDirectArray().GetAt(tangentIndex);
                    }
                } else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
                    const int vertexIndex = mesh->GetPolygonVertexIndex(i) + j;
                    if (referenceMode == FbxGeometryElement::eDirect) {
                        tangent = tangentElement->GetDirectArray().GetAt(vertexIndex);
                    } else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                        const int tangentIndex = tangentElement->GetIndexArray().GetAt(vertexIndex);
                        tangent = tangentElement->GetDirectArray().GetAt(tangentIndex);
                    }
                } else {
                    Log::error(m_Path, " Error: Invalid mappingMode!");
                }

                tangents.push_back(static_cast<float>(tangent[0]));
                tangents.push_back(static_cast<float>(tangent[1]));
                tangents.push_back(static_cast<float>(tangent[2]));
            }

            indices.push_back(index);
            index++;
        }
    }
}
