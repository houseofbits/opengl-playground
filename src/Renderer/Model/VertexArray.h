#pragma once

#include <vector>

class VertexArray {
public:
    VertexArray();

    virtual ~VertexArray();

    virtual void draw();

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
