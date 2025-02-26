#pragma once

#include "../../../libs/tinygltf/json.hpp"
#include "Component.h"
#include "Entity.h"
#include "../Helper/StringUtils.h"

class EntitySerializer {
public:
    static void serialize(Entity &e, nlohmann::json &json) {
        json["name"] = e.m_Name;
        for (auto const &comp: e.m_Components) {
            nlohmann::json compJson;
            comp->serialize(compJson);
            json[comp->getNameKey()] = compJson;
        }
    }

    static void deserialize(Entity &e, nlohmann::json &json, ResourceManager &resourceManager) {
        if (json.contains("name")) {
            e.m_Name = json["name"];
        }
        auto emptyJson = nlohmann::json({});
        for (auto const &comp: e.m_Components) {
            try {
                if (json.contains(comp->getNameKey())) {
                    comp->deserialize(json[comp->getNameKey()], resourceManager);
                } else {
                    comp->deserialize(emptyJson, resourceManager);
                }

                comp->m_Status = Component::STATUS_DESERIALIZED;
            } catch (nlohmann::detail::type_error &exception) {
                std::cout << "JSON deserialization error " << e.m_Name << ": " << exception.what() << std::endl;
            }
        }
    }

    static std::string getComponentNameFromNameKey(const std::string &nameKey) {
        auto tokens = StringUtils::splitString(nameKey, '#');

        if (tokens.size() > 1) {
            return tokens[1];
        }

        return "";
    }

    static std::string getComponentTypeFromNameKey(const std::string &nameKey) {
        auto tokens = StringUtils::splitString(nameKey, '#');

        return tokens[0];
    }

    static std::pair<std::string, std::string> getComponentTypeAndNameFromNameKey(const std::string &nameKey) {
        return {
            getComponentTypeFromNameKey(nameKey),
            getComponentNameFromNameKey(nameKey),
        };
    }
};
