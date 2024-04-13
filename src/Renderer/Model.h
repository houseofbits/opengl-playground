#pragma once
#include <string>
#include <map>
#include <utility>
#include <list>
#include <GL/glew.h>
#include "../../libs/tinygltf/tiny_gltf.h"
#include "VertexArray.h"

class Model : public VertexArray
{
public:
    void createFromFile(std::string filename);
};