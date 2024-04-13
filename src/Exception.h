#pragma once
#include <string>

class Exception
{
private:
    int code;
    std::string name;

public:
    Exception(int code, std::string name = "") : name(name), code(code) {}

    virtual const char *what() const throw()
    {
        return name.c_str();
    }
};