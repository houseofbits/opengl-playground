#include "EntityConfiguration.h"
#include "../../Helper/Log.h"
#include "Entity.h"

EntityConfiguration::EntityConfiguration(Factory<Component> &factory) : m_EntityConfiguration(), m_ComponentFactory(factory) {
}

void EntityConfiguration::buildEntity(Entity &entity, const std::string &configurationName, ResourceManager &resourceManager) {
    entity.m_TypeName = configurationName;
    auto it = m_EntityConfiguration.find(configurationName);
    if (it == m_EntityConfiguration.end()) {
        Log::error("EntityConfiguration::buildEntity: Entity configuration not found " + configurationName);
        return;
    }

    for (auto const &componentConfig: (*it).second) {
        Component *c = m_ComponentFactory.createInstance(componentConfig.second.m_ClassName);
        if (c == nullptr) {
            Log::error("EntityConfiguration::buildEntity: Component constructor not found " + componentConfig.second.m_ClassName);
            break;
        }
        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_Name = componentConfig.second.m_Name;
        c->m_EntityId = entity.m_Id;

        if (!componentConfig.second.m_DefaultJson.empty()) {
            c->deserialize(componentConfig.second.m_DefaultJson, resourceManager);
        }

        entity.addComponent(*c);
    }
}

void EntityConfiguration::deserialize(nlohmann::json &json) {
    for (auto &el: json.items()) {
        TComponentConfiguration config;

        if (!el.value().contains("components")) {
            Log::error("EntityConfiguration::deserialize: components is undefined");
            return;
        }

        for (auto &comp: el.value().at("components").items()) {
            if (!comp.value().contains("class")) {
                Log::error("EntityConfiguration::deserialize: class is undefined");
                return;
            }
            ComponentRecord componentRecord;
            componentRecord.m_Name = comp.key();
            componentRecord.m_ClassName = comp.value().at("class");

            if (comp.value().contains("default")) {
                componentRecord.m_DefaultJson = comp.value().at("default");
            }

            if (config.find(comp.key()) != config.end()) {
                Log::error("EntityConfiguration::deserialize: component already defined");
                return;
            }

            config[comp.key()] = componentRecord;
        }

        std::string name = el.value().at("name");
        if (m_EntityConfiguration.find(name) != m_EntityConfiguration.end()) {
            Log::error("EntityConfiguration::deserialize: entity configuration already defined");
            return;
        }

        m_EntityConfiguration[name] = config;
    }
}
