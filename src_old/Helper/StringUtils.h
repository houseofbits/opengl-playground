#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace StringUtils {
    std::string replaceAll(std::string str, const std::string &from, const std::string &to);
    std::vector<std::string> getAlternatePaths(const std::string &input);
}// namespace StringUtils