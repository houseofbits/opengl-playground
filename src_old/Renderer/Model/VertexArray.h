#pragma once
#include <vector>

class VertexArray
{
public:
    virtual void draw();
    unsigned int vertexArrayObjectId{0};
    struct Element
    {
        int mode;
        int count;
        int componentType;
        int bufferOffset;
        unsigned int bufferId;
    };

    unsigned int numberOfVerts{0};

    std::vector<Element> elementsArray;
    std::vector<unsigned int> bufferIds;

    void createBuffers(
        std::vector<unsigned int> *indices,
        std::vector<float> *vertices,
        std::vector<float> *normals = nullptr,
        std::vector<float> *texCoords = nullptr,
        std::vector<float> *tangents = nullptr);

    void deleteBuffers();

private:
    unsigned int generateIndexBuffer(std::vector<unsigned int> *array);
    unsigned int generateFloatBuffer(std::vector<float> *array);


};
