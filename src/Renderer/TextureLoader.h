#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Texture.h"

class TextureLoader
{
public:
    static Texture load(std::string filename);
    static bool loadData(std::string filename, int *width, int *height, unsigned char *&data);
};