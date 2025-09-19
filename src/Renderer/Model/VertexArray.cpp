#include "VertexArray.h"
#include <GL/glew.h>
#include "../../Core//Helper/Log.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexArray::VertexArray() : elementsArray(), bufferIds() {

}

VertexArray::~VertexArray() {
    deleteBuffers();
}

void VertexArray::createBuffers(
    std::vector<unsigned int> *indices,
    std::vector<float> *vertices,
    std::vector<float> *normals,
    std::vector<float> *texCoords,
    std::vector<float> *tangents,
    std::vector<float> *colors)
{
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    generateIndexBuffer(indices);

    generateFloatBuffer(vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    if (normals != nullptr)
    {
        generateFloatBuffer(normals);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
    }

    if (texCoords != nullptr)
    {
        generateFloatBuffer(texCoords);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
    }

    if (tangents != nullptr)
    {
        generateFloatBuffer(tangents);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(3);
    }

    if (colors != nullptr)
    {
        generateFloatBuffer(colors);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(4);
    }

    glBindVertexArray(0);
}

void VertexArray::deleteBuffers()
{
    if (!bufferIds.empty())
    {
        glDeleteBuffers((GLsizei)bufferIds.size(), bufferIds.data());
        bufferIds.clear();
    }

    if (vertexArrayObjectId != 0)
    {
        glDeleteVertexArrays(1, &vertexArrayObjectId);
        vertexArrayObjectId = 0;
    }
}

unsigned int VertexArray::generateIndexBuffer(std::vector<unsigned int> *array)
{
    unsigned int bufferId = 0;

    glGenBuffers(1, &bufferId);
    bufferIds.push_back(bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array->size() * sizeof(GLuint), array->data(), GL_STATIC_DRAW);

    auto el = new Element();
    el->bufferId = bufferId;
    el->mode = GL_TRIANGLES;
    el->count = static_cast<int>(array->size());
    el->componentType = GL_UNSIGNED_INT;
    el->bufferOffset = 0;

    elementsArray.push_back(el);

    return bufferId;
}

unsigned int VertexArray::generateFloatBuffer(std::vector<float> *array)
{
    unsigned int bufferId = 0;

    glGenBuffers(1, &bufferId);
    bufferIds.push_back(bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, array->size() * sizeof(GLfloat), array->data(), GL_STATIC_DRAW);

    return bufferId;
}

void VertexArray::draw()
{
    if (vertexArrayObjectId)
    {
        glBindVertexArray(vertexArrayObjectId);

        for (const auto &element : elementsArray)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->bufferId);

            glDrawElements(element->mode, element->count,
                           element->componentType,
                           BUFFER_OFFSET(element->bufferOffset));
        }

        glBindVertexArray(0);
    }
}