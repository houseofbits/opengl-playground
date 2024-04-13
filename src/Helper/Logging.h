#pragma once
#include <string>
#include <iostream>

class Logging
{
public:
    static void write(std::string message)
    {
        std::cout << message << std::endl;
    }
};