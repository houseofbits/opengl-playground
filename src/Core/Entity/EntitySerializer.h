#pragma once
#include "../../../libs/tinygltf/json.hpp"
#include "Component.h"
#include "Entity.h"

class EntitySerializer {
public:
    static void serialize(Entity &e, nlohmann::json &json) {
        json["name"] = e.m_Name;
        for (auto const &comp: e.m_Components) {
            nlohmann::json compJson;
            comp->serialize(compJson);
            json[comp->m_Name] = compJson;
        }
    }

    static void deserialize(Entity &e, nlohmann::json &json, ResourceManager &resourceManager) {
        if (json.contains("name")) {
            e.m_Name = json["name"];
        }
        auto emptyJson = nlohmann::json({});
        for (auto const &comp: e.m_Components) {
            if (json.contains(comp->m_Name)) {
                comp->deserialize(json[comp->m_Name], resourceManager);
            } else {
                comp->deserialize(emptyJson, resourceManager);
            }
        }
    }
};
