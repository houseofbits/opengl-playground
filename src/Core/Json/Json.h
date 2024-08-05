#pragma once

#include "../../../libs/tinygltf/json.hpp"
#include <glm/ext/quaternion_float.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <optional>

namespace nlohmann {

    void to_json(nlohmann::json &j, const glm::vec2 &p);
    void from_json(const nlohmann::json &j, glm::vec2 &p);

    void to_json(nlohmann::json &j, const glm::vec3 &p);
    void from_json(const nlohmann::json &j, glm::vec3 &p);

    void to_json(nlohmann::json &j, const glm::vec4 &p);
    void from_json(const nlohmann::json &j, glm::vec4 &p);

    void to_json(nlohmann::json &j, const glm::quat &p);
    void from_json(const nlohmann::json &j, glm::quat &p);

    template<class T>
    void to_json(nlohmann::json &j, const std::optional<T> &v) {
        if (v.has_value())
            j = *v;
        else
            j = nullptr;
    }

    template<class T>
    void from_json(const nlohmann::json &j, std::optional<T> &v) {
        if (j.is_null())
            v = std::nullopt;
        else
            v = j.get<T>();
    }

    template<class J, class T>
    void optional_to_json(J &j, const char *name, const std::optional<T> &value) {
        if (value) {
            j[name] = *value;
        }
    }

    template<class J, class T>
    void optional_from_json(const J &j, const char *name, std::optional<T> &value) {
        const auto it = j.find(name);
        if (it != j.end()) {
            value = it->template get<T>();
        } else {
            value = std::nullopt;
        }
    }

    template<typename>
    constexpr bool is_optional = false;
    template<typename T>
    constexpr bool is_optional<std::optional<T>> = true;

    template<typename T>
    void extended_to_json(const char *key, nlohmann::json &j, const T &value) {
        if constexpr (is_optional<T>)
            optional_to_json(j, key, value);
        else
            j[key] = value;
    }
    template<typename T>
    void extended_from_json(const char *key, const nlohmann::json &j, T &value) {
        if constexpr (is_optional<T>)
            optional_from_json(j, key, value);
        else
            j.at(key).get_to(value);
    }

#define EXTEND_JSON_TO(v1) extended_to_json(#v1, nlohmann_json_j, nlohmann_json_t.v1);
#define EXTEND_JSON_FROM(v1) extended_from_json(#v1, nlohmann_json_j, nlohmann_json_t.v1);
#define NLOHMANN_JSONIFY_TYPE(Type, ...)                                            \
    inline void to_json(nlohmann::json &nlohmann_json_j, const Type &nlohmann_json_t) {   \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(EXTEND_JSON_TO, __VA_ARGS__))            \
    }                                                                                     \
    inline void from_json(const nlohmann::json &nlohmann_json_j, Type &nlohmann_json_t) { \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(EXTEND_JSON_FROM, __VA_ARGS__))          \
    }
}// namespace nlohmann