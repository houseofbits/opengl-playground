#include "StringUtils.h"
#include <algorithm>

namespace StringUtils {
    std::string replaceAll(std::string str, const std::string &from, const std::string &to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    std::vector<std::string> getAlternatePaths(const std::string &input) {
        std::vector<std::string> result;

        std::stringstream stream(input);
        std::vector<std::string> parts;
        std::string part;
        while (std::getline(stream, part, '|')) {
            parts.push_back(part);
        }

        std::string path = parts.front();
        parts.erase(parts.begin());

        for (const auto &replacement: parts) {
            result.push_back(path + replacement);
        }

        std::sort(result.begin(), result.end());

        if (result.empty()) {
            result.push_back(input);
        }

        return result;
    }
};// namespace StringUtils
