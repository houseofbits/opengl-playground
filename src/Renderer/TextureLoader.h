#pragma once

#include "Texture.h"
#include "TextureCube.h"
#include <GL/glew.h>
#include <iostream>
#include <string>

class TextureLoader {
public:
    inline static const std::string CUBE_FACE_FILENAME_PLACEHOLDER = "*";
    inline static const std::string CUBE_FACE_FILENAME_SUFFIXES[6] = {
            "px",
            "nx",
            "py",
            "ny",
            "pz",
            "nz",
    };
    static Texture load(std::string filename, bool withMipmapping = false);
    static TextureCube loadCube(std::string filename, bool withMipmapping = false);
    static bool loadData(std::string filename, int *width, int *height, unsigned char *&data);
    static void freeData(void *data);
};