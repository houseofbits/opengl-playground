#include "LineMesh.h"
#include <GL/glew.h>
#include <iostream>

void LineMesh::setColor(glm::vec4 color)
{
    currentColor = color;
}

void LineMesh::addVertex(glm::vec3 vertex)
{
    colorArray.push_back(currentColor);
    vertexArray.push_back(vertex);
}

void LineMesh::build()
{
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    unsigned int vertexBufferId = 0;
    glGenBuffers(1, &vertexBufferId);
    bufferIds.push_back(vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(GLfloat) * 3, vertexArray.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    unsigned int colorBufferId = 0;
    glGenBuffers(1, &colorBufferId);
    bufferIds.push_back(colorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
    glBufferData(GL_ARRAY_BUFFER, colorArray.size() * sizeof(GLfloat) * 4, colorArray.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    numberOfVerts = vertexArray.size();

    vertexArray.clear();
    colorArray.clear();
}

void LineMesh::draw()
{
    glBindVertexArray(vertexArrayObjectId);
    glDrawArrays(GL_LINES, 0, numberOfVerts);
    glBindVertexArray(0);
}