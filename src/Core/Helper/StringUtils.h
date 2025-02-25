#pragma once

#include <algorithm>
#include <glm/vec3.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace StringUtils {
    std::string replaceAll(std::string str, const std::string &from, const std::string &to);

    std::vector<std::string> getAlternatePaths(const std::string &input);

    template<typename T>
    void removeSubstring(std::basic_string<T> &s,
                         const std::basic_string<T> &p) {
        typename std::basic_string<T>::size_type n = p.length();

        for (typename std::basic_string<T>::size_type i = s.find(p);
             i != std::basic_string<T>::npos;
             i = s.find(p))
            s.erase(i, n);
    }

    inline std::string toString(glm::vec3 v) {
        return std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
    }

    inline std::string toLowerCase(const std::string &s) {
        std::string str(s);
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        return s;
    }

    inline bool contains(const std::string &source, const std::string &needle) {
        return StringUtils::toLowerCase(source).find(StringUtils::toLowerCase(needle)) !=
               std::string::npos;
    }

    inline std::string capitalizeFirstLetter(const std::string& input) {
        if (input.empty()) return input;

        std::string result = input;
        result[0] = std::toupper(static_cast<unsigned char>(result[0]));
        return result;
    }

    inline std::string pascalCaseToHumanReadable(const std::string text) {
        std::string result;
        for (size_t i = 0; i < text.size(); i++) {
            if (i > 0 && std::isupper(text[i]) && (std::islower(text[i - 1]) || std::isdigit(text[i - 1]))) {
                result += ' ';
            }
            result += text[i];
        }

        return result;
    }
} // namespace StringUtils
