#pragma once

#include "../../Helper/Console.h"
#include "../../Window.h"
#include "../../Renderer/Camera.h"
#include <iostream>
#include <string>

class BaseRenderTest
{
public:
    static void message(const std::string& message)
    {
        std::cout
            << BACKGRN
            << FOREBLK
            << message
            << RESETTEXT
            << std::endl;
    }

    virtual void setup(Window &window) = 0;
    virtual void render(Camera&) = 0;
    virtual void destroy() = 0;
};