
#include "BaseConfiguration.h"

namespace nlohmann {
    void to_json(nlohmann::json &j, const glm::vec3 &p) {
        j = nlohmann::json{p.x, p.y, p.z};
    }

    void from_json(const nlohmann::json &j, glm::vec3 &p) {
        j.at(0).get_to(p.x);
        j.at(1).get_to(p.y);
        j.at(2).get_to(p.z);
    }

    void to_json(nlohmann::json &j, const glm::vec4 &p) {
        j = nlohmann::json{p.x, p.y, p.z, p.w};
    }

    void from_json(const nlohmann::json &j, glm::vec4 &p) {
        j.at(0).get_to(p.x);
        j.at(1).get_to(p.y);
        j.at(2).get_to(p.z);
        j.at(3).get_to(p.w);
    }
}// namespace nlohmann