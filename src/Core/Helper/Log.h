#pragma once
#include "Console.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

class Log {
public:
    static void write(std::string message) {
        std::cout << message << std::endl;
    }

    static void error(const std::string &message) {
        std::cout << FORERED << message << RESETTEXT << std::endl;
    }

    static void warn(const std::string &message) {
        std::cout << FOREYEL << message << RESETTEXT << std::endl;
    }

    static void info(const std::string &message) {
        std::cout << FOREWHT << message << RESETTEXT << std::endl;
    }

    static void write(glm::vec3 v) {
        std::cout << v.x << "," << v.y << "," << v.z << std::endl;
    }
};