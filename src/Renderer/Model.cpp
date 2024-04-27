#include "Model.h"
#include <iostream>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Model::createFromFile(std::string filename)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    if (!warn.empty())
    {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
    {
        std::cout << "Failed to load model: " << filename << std::endl;
    }

    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    std::map<int, GLuint> generatedBuffers;
    for (size_t i = 0; i < model.bufferViews.size(); ++i)
    {
        const tinygltf::BufferView &bufferView = model.bufferViews[i];
        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

        GLuint vbo;
        glGenBuffers(1, &vbo);
        generatedBuffers[i] = vbo;
        glBindBuffer(bufferView.target, vbo);
        glBufferData(bufferView.target, bufferView.byteLength,
                     &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (const auto &node : model.nodes)
    {
        if (node.mesh >= 0)
        {
            for (const auto &primitive : model.meshes[node.mesh].primitives)
            {
                if (primitive.indices < 0)
                {
                    continue;
                }

                tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

                Element el;
                el.mode = primitive.mode;
                el.count = indexAccessor.count;
                el.componentType = indexAccessor.componentType;
                el.bufferOffset = indexAccessor.byteOffset;
                el.bufferId = generatedBuffers[indexAccessor.bufferView];
                elementsArray.push_back(el);

                for (auto &attrib : primitive.attributes)
                {
                    tinygltf::Accessor accessor = model.accessors[attrib.second];

                    int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                    glBindBuffer(GL_ARRAY_BUFFER, generatedBuffers[accessor.bufferView]);

                    int size = 1;
                    if (accessor.type != TINYGLTF_TYPE_SCALAR)
                    {
                        size = accessor.type;
                    }

                    int vaa = -1;
                    if (attrib.first.compare("POSITION") == 0)
                        vaa = 0;
                    if (attrib.first.compare("NORMAL") == 0)
                        vaa = 1;
                    if (attrib.first.compare("TEXCOORD_0") == 0)
                        vaa = 2;
                    if (vaa > -1)
                    {
                        glEnableVertexAttribArray(vaa);
                        glVertexAttribPointer(vaa, size, accessor.componentType,
                                              accessor.normalized ? GL_TRUE : GL_FALSE,
                                              byteStride, BUFFER_OFFSET(accessor.byteOffset));
                    }
                }
            }
        }
    }

    glBindVertexArray(0);

    // for (auto it = generatedBuffers.cbegin(); it != generatedBuffers.cend();)
    // {
    //     tinygltf::BufferView bufferView = model.bufferViews[it->first];
    //     if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER)
    //     {
    //         glDeleteBuffers(1, &generatedBuffers[it->first]);
    //         generatedBuffers.erase(it++);
    //     }
    //     else
    //     {
    //         ++it;
    //     }
    // }
}