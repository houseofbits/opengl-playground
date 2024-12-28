#pragma once
#include <string>
#include <utility>

class Exception : public std::exception
{
private:
    int code;
    std::string name;

public:
    explicit Exception(int code, std::string name = "") : name(std::move(name)), code(code) {}

    [[nodiscard]] const char *what() const noexcept override
    {
        return name.c_str();
    }
};