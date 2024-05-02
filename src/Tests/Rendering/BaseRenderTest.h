#pragma once

#include "../../Helper/Console.h"
#include "../../Window.h"
#include <iostream>
#include <string>

class BaseRenderTest
{
public:
    void message(std::string message)
    {
        std::cout
            << BACKGRN
            << FOREBLK
            << message
            << RESETTEXT
            << std::endl;
    }

    virtual void setup(Window &window) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};