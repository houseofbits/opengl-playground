#include "RenderModelBuilder.h"
#include <GL/glew.h>
#include "../../Core/Helper/Log.h"

RenderModelBuilder::RenderModelBuilder() = default;

void RenderModelBuilder::buildFromModelConfiguration(
    const ModelConfigurationLoader::ModelConfiguration &modelConfigurationIn,
    Model &modelOut) {
    glGenVertexArrays(1, &modelOut.m_vertexArrayObject);
    glGenBuffers(1, &modelOut.m_vertexBufferObject);
    glGenBuffers(1, &modelOut.m_elementBufferObject);

    glBindVertexArray(modelOut.m_vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, modelOut.m_vertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizei>(modelConfigurationIn.vertices.size() * sizeof(VertexArray::Vertex)),
        modelConfigurationIn.vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelOut.m_elementBufferObject);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizei>(modelConfigurationIn.indices.size() * sizeof(unsigned int)),
        modelConfigurationIn.indices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexArray::Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexArray::Vertex),
                          reinterpret_cast<void *>(offsetof(VertexArray::Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexArray::Vertex),
                          reinterpret_cast<void *>(offsetof(VertexArray::Vertex, texCoord)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexArray::Vertex),
                          reinterpret_cast<void *>(offsetof(VertexArray::Vertex, tangent)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexArray::Vertex),
                          reinterpret_cast<void *>(offsetof(VertexArray::Vertex, biTangent)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

    // Log::write(modelConfigurationIn.indices.size(), " ", modelConfigurationIn.vertices.size());
    // for (const auto& node: modelConfigurationIn.nodes) {
    //     Log::write(node.name, ", ", node.offset, ", ", node.size);
    // }
    //
    // for (const auto& index: modelConfigurationIn.indices) {
    //     auto& vertex = modelConfigurationIn.vertices[index];
    //     Log::write(index, " ", vertex.position, "   ", vertex.normal);
    // }

    modelOut.m_meshNodes = modelConfigurationIn.nodes;
}
