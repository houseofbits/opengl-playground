#pragma once

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
}// namespace StringUtils