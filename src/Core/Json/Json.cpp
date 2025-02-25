#include "Json.h"

namespace nlohmann {
    void to_json(nlohmann::json &j, const glm::vec2 &p) {
        j = nlohmann::json{p.x, p.y};
    }

    void from_json(const nlohmann::json &j, glm::vec2 &p) {
        if (j.is_number()) {
            p = glm::vec2(j.get<float>());
        } else if (j.is_array()) {
            if (j.size() == 1) {
                p = glm::vec2(j.at(0).get<float>());
            } else if (j.size() >= 2) {
                j.at(0).get_to(p.x);
                j.at(1).get_to(p.y);
            }
        }
    }

    void to_json(nlohmann::json &j, const glm::vec3 &p) {
        j = nlohmann::json{p.x, p.y, p.z};
    }

    void from_json(const nlohmann::json &j, glm::vec3 &p) {
        if (j.is_number()) {
            p = glm::vec3(j.get<float>());
        } else if (j.is_array()) {
            if (j.size() == 1) {
                p = glm::vec3(j.at(0).get<float>());
            } else if (j.size() >= 3) {
                j.at(0).get_to(p.x);
                j.at(1).get_to(p.y);
                j.at(2).get_to(p.z);
            }
        }
    }

    void to_json(nlohmann::json &j, const glm::vec4 &p) {
        j = nlohmann::json{p.x, p.y, p.z, p.w};
    }

    void from_json(const nlohmann::json &j, glm::vec4 &p) {
        if (j.is_number()) {
            p = glm::vec4(j.get<float>());
        } else if (j.is_array()) {
            if (j.size() == 1) {
                p = glm::vec4(j.at(0).get<float>());
            } else if (j.size() >= 4) {
                j.at(0).get_to(p.x);
                j.at(1).get_to(p.y);
                j.at(2).get_to(p.z);
                j.at(3).get_to(p.w);
            }
        }
    }

    void to_json(nlohmann::json &j, const glm::quat &p) {
        j = nlohmann::json{p.x, p.y, p.z, p.w};
    }

    void from_json(const nlohmann::json &j, glm::quat &p) {
        if (j.is_array()) {
            if (j.size() >= 4) {
                j.at(0).get_to(p.x);
                j.at(1).get_to(p.y);
                j.at(2).get_to(p.z);
                j.at(3).get_to(p.w);
            }
        }
    }
}// namespace nlohmann