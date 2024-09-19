#pragma once
#include "Console.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

class Log {
public:
    template<typename... Rest>
    static void write(const Rest &...rest) {
        print(rest...);
        std::cout << std::endl;
    }

    template<typename... Rest>
    static void error(const Rest &...rest) {
        std::cout << FORERED;
        print(rest...);
        std::cout << RESETTEXT << std::endl;
    }

    template<typename... Rest>
    static void warn(const Rest &...rest) {
        std::cout << FOREYEL;
        print(rest...);
        std::cout << RESETTEXT << std::endl;
    }

    template<typename... Rest>
    static void info(const Rest &...rest) {
        std::cout << FOREWHT;
        print(rest...);
        std::cout << RESETTEXT << std::endl;
    }

private:
    template<typename T>
    static void print(const T &t) {
        std::cout << t;
    }

    static void print(const glm::vec2 &t) {
        std::cout << t.x << "," << t.y;
    }

    static void print(const glm::vec3 &t) {
        std::cout << t.x << "," << t.y << "," << t.z;
    }

    template<typename First, typename... Rest>
    static void print(const First &first, const Rest &...rest) {
        print(first);
        print(rest...);
    }
};