#pragma once

#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class WireModel {
public:
    WireModel() = default;

    void setColor(glm::vec4);

    void addVertex(glm::vec3);

    void addSegment(glm::vec3, glm::vec3);

    void build();

    void draw() const;

protected:
    void createCircle(const glm::mat4 &transform, float radius, unsigned int segments);

private:
    void rebuild() const;

    void deleteBuffers();

    unsigned int m_numberOfVertices{0};
    unsigned int m_vertexArrayObjectId{0};
    std::vector<unsigned int> m_bufferIds;

    glm::vec4 m_currentColor{1.0f};
    std::vector<glm::vec4> m_colorArray;
    std::vector<glm::vec3> m_vertexArray;
};
