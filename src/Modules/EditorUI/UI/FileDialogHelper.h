#pragma once

#include <string>

bool FileInput(const std::string &name, const std::string &title, const std::string &ext, const std::string &label,
               std::string &path, const std::string &displayPath = "");