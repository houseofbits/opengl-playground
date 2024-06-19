#pragma once

#include "../../../libs/tinygltf/json.hpp"
#include "../Reflection/Factory.h"
#include "Component.h"
#include "Entity.h"
#include <list>
#include <map>
#include <optional>
#include <string>

class EntityConfiguration {
public:
    explicit EntityConfiguration(Factory<Component> &factory);

    class ComponentRecord {
    public:
        std::string m_Name;
        std::string m_ClassName;
        nlohmann::json m_DefaultJson;
    };

    typedef std::map<std::string, ComponentRecord> TComponentConfiguration;
    typedef std::map<std::string, TComponentConfiguration> TEntityConfiguration;

    TEntityConfiguration m_EntityConfiguration;
    Factory<Component> &m_ComponentFactory;

    void deserialize(nlohmann::json &json);
    void buildEntity(Entity &entity, const std::string &configurationName, ResourceManager &resourceManager);
};
