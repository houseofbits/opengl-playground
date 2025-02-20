#include "WireModel.h"
#include <GL/glew.h>
#include <iostream>
#include <cmath>

void WireModel::setColor(glm::vec4 color) {
    m_currentColor = color;
}

void WireModel::addVertex(const glm::vec3 vertex) {
    m_colorArray.push_back(m_currentColor);
    m_vertexArray.push_back(vertex);
}

void WireModel::addSegment(const glm::vec3 a, const glm::vec3 b) {
    addVertex(a);
    addVertex(b);
}

void WireModel::createCircle(const glm::mat4 &transform, float radius, unsigned int segments) {
    glm::vec3 prevP = {};
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glm::vec4 p4 = {
            radius * cos(angle),
            radius * sin(angle),
            0.0f,
            1.0f
        };

        glm::vec3 p = glm::vec3(transform * p4);

        if (i > 0) {
            addSegment(prevP, p);
        }

        prevP = p;
    }
}

void WireModel::build() {
    glGenVertexArrays(1, &m_vertexArrayObjectId);
    glBindVertexArray(m_vertexArrayObjectId);

    unsigned int vertexBufferId = 0;
    glGenBuffers(1, &vertexBufferId);
    m_bufferIds.push_back(vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertexArray.size() * sizeof(GLfloat) * 3),
                 m_vertexArray.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    unsigned int colorBufferId = 0;
    glGenBuffers(1, &colorBufferId);
    m_bufferIds.push_back(colorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_colorArray.size() * sizeof(GLfloat) * 4),
                 m_colorArray.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_numberOfVertices = m_vertexArray.size();

    m_vertexArray.clear();
    m_colorArray.clear();
}

void WireModel::draw() const {
    glBindVertexArray(m_vertexArrayObjectId);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_numberOfVertices));
    glBindVertexArray(0);
}
