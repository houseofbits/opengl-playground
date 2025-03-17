#pragma once

#include <fstream>

namespace File {
    inline bool exists(const std::string &filename) {
        std::ifstream file(filename);
        return file.good();
    }
}
