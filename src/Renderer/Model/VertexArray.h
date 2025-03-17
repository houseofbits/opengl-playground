#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class VertexArray {
public:
    VertexArray();

    virtual ~VertexArray();

    virtual void draw();

    struct Vertex {
        glm::vec3 position{1.0};
        glm::vec3 normal{0.0};
        glm::vec2 texCoord{0.0};
        glm::vec3 tangent{0.0};
        glm::vec3 biTangent{0.0};
    };

    unsigned int vertexArrayObjectId{0};
    struct Element {
        int mode{0};
        unsigned int count{0};
        int componentType{0};
        unsigned int bufferOffset{0};
        unsigned int bufferId{0};
        unsigned int vaoId{0};
    };

    std::vector<Element*> elementsArray{};
    std::vector<unsigned int> bufferIds{};

    void createBuffers(
            std::vector<unsigned int> *indices,
            std::vector<float> *vertices,
            std::vector<float> *normals = nullptr,
            std::vector<float> *texCoords = nullptr,
            std::vector<float> *tangents = nullptr,
            std::vector<float> *colors = nullptr
    );

    void deleteBuffers();

private:
    unsigned int generateIndexBuffer(std::vector<unsigned int> *array);

    unsigned int generateFloatBuffer(std::vector<float> *array);
};
