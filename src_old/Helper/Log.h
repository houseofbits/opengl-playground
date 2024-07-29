#pragma once
#include <string>
#include <iostream>
#include "Console.h"

class Log {
public:
    static void write(std::string message)
    {
        std::cout << message << std::endl;
    }

    static void error(const std::string& message)
    {
        std::cout << FORERED << message <<  RESETTEXT << std::endl;
    }

    static void warn(const std::string& message)
    {
        std::cout << FOREYEL << message <<  RESETTEXT << std::endl;
    }

    static void info(const std::string& message)
    {
        std::cout << FOREWHT << message <<  RESETTEXT << std::endl;
    }
};