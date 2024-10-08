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
        if (!e.m_Behaviours.empty()) {
            nlohmann::json behavioursJson = nlohmann::json::object();
            for (auto const &behaviour: e.m_Behaviours) {
                nlohmann::json behaviourJson;
                behaviour->serialize(behaviourJson);
                behavioursJson[behaviour->getTypeName()] = behaviourJson;
            }
            json["behaviours"] = behavioursJson;
        }
    }

    static void deserialize(Entity &e, nlohmann::json &json, ResourceManager &resourceManager) {
        if (json.contains("name")) {
            e.m_Name = json["name"];
        }
        auto emptyJson = nlohmann::json({});
        for (auto const &comp: e.m_Components) {
            try {
                if (json.contains(comp->m_Name)) {
                    comp->deserialize(json[comp->m_Name], resourceManager);
                } else {
                    comp->deserialize(emptyJson, resourceManager);
                }
            } catch (nlohmann::detail::type_error &exception) {
                std::cout << "JSON deserialization error " << e.m_Name << ": " << exception.what() << std::endl;
            }
        }
        for (auto const &behaviour: e.m_Behaviours) {
            if (json.contains("behaviours") && json["behaviours"].is_object()) {
                if (json["behaviours"].contains(behaviour->getTypeName())) {
                    behaviour->deserialize(json["behaviours"][behaviour->getTypeName()], resourceManager);
                } else {
                    behaviour->deserialize(emptyJson, resourceManager);
                }
            }
        }
    }
};
