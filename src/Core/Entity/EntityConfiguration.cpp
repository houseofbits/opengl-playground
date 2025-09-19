#include "EntityConfiguration.h"
#include "../../Core/Helper/Log.h"
#include "Entity.h"
#include "EntitySerializer.h"

EntityConfiguration::EntityConfiguration(Factory<Component> &factory)
    : m_EntityConfiguration(), m_ComponentFactory(factory) {
}

void EntityConfiguration::buildEntity(Entity &entity, const std::string &configurationName,
                                      ResourceManager &resourceManager) {
    auto it = m_EntityConfiguration.find(configurationName);
    if (it == m_EntityConfiguration.end()) {
        Log::error("EntityConfiguration::buildEntity: Entity configuration not found " + configurationName);
        return;
    }

    for (const auto &[fst, snd]: (*it).second) {
        Component *c = m_ComponentFactory.createInstance(snd.m_ClassName);
        if (c == nullptr) {
            Log::error("EntityConfiguration::buildEntity: Component constructor not found " +
                       snd.m_ClassName);
            break;
        }
        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_EntityId = entity.m_Id;
        c->m_Name = snd.m_Name;

        if (!snd.m_DefaultJson.empty()) {
            c->deserialize(snd.m_DefaultJson, resourceManager);
        }

        c->m_Status = Component::STATUS_DESERIALIZED;

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

            const auto [type, name] = EntitySerializer::getComponentTypeAndNameFromNameKey(comp.key());

            ComponentRecord componentRecord;
            componentRecord.m_Name = name;
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

std::vector<std::string> EntityConfiguration::getAllConfigurationNames() {
    std::vector<std::string> names;

    for (const auto &configuration: m_EntityConfiguration) {
        names.push_back(configuration.first);
    }

    return names;
}
