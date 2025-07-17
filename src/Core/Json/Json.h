#pragma once
#include <fstream>
#include "Json.h"
#include "../Helper/Log.h"

namespace Json {
    inline std::optional<nlohmann::json> readFile(const std::string &filePath) {
        std::ifstream file(filePath);
        if (file.fail()) {
            Log::error("Failed to read JSON file: " + filePath);

            return std::nullopt;
        }
        return nlohmann::json::parse(file);
    }

    inline void writeFile(const std::string &filePath, const nlohmann::json &j) {
        std::ofstream file;
        file.open(filePath);

        const std::string data(j.dump(4));
        file << data;
    }

    template<class T>
    inline T getValue(nlohmann::json json, std::string key, T defaultValue) {
        if (json.contains(key) && !json[key].is_null()) {
            return json.value(key, defaultValue);
        }
        return defaultValue;
    }
};
