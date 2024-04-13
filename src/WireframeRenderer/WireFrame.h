#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class WireFrame
{
private:
    unsigned int numberOfVerts;
    unsigned int vertexArrayObjectId;
    std::vector<unsigned int> bufferIds;

    glm::vec4 currentColor;
    std::vector<glm::vec4> colorArray;
    std::vector<glm::vec3> vertexArray;

public:
    void setColor(glm::vec4);
    void addVertex(glm::vec3);
    void build();
    void draw();
};