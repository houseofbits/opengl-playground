#pragma once
#include "../../SourceLibs/tinygltf/json.hpp"
#include "Component.h"
#include "Entity.h"

class EntitySerializer {
public:
    static void serialize(Entity &e, nlohmann::json &json) {
        json["name"] = e.m_Name;
        json["type"] = e.m_TypeName;
        for (auto const &comp: e.m_Components) {
            nlohmann::json compJson;
            comp->serialize(compJson);
            json[comp->m_Name] = compJson;
        }
    }

    static void deserialize(Entity &e, nlohmann::json &json, ResourceManager &resourceManager) {
        if (!json.contains("type")) {
            Log::error("EntitySerializer::deserialize: Json does not contain entity type");
            return;
        }

        e.m_TypeName = json["type"];

        if (json.contains("name")) {
            e.m_Name = json["name"];
        }
        for (auto const &comp: e.m_Components) {
            if (json.contains(comp->m_Name)) {
                comp->deserialize(json[comp->m_Name], resourceManager);
            }
        }
    }
};
