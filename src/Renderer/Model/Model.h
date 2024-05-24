#pragma once
#include "../../../libs/tinygltf/tiny_gltf.h"
#include "VertexArray.h"
#include <GL/glew.h>
#include <list>
#include <map>
#include <string>
#include <utility>

class Model : public VertexArray
{
public:
    void createFromFile(std::string filename);
};