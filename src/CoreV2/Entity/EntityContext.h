#pragma once

#include "../Module/EntityModule.h"
#include "../Reflection/Factory.h"
#include "../Resources/ResourceManager.h"
#include "../System/EntitySystem.h"
#include "Component.h"
#include "Entity.h"
#include "EntityConfiguration.h"
#include <memory>

//class EntityModule;


//0. add empty entity
//1. buildEntity - populate with components
//2. register systems
//3. deserialize or populate initial data
//4. gather resources
//5. initialize
class EntityContext {
private:
    Factory<Component> m_ComponentFactory;
    EntityConfiguration m_EntityConfiguration;

    std::list<Entity::TEntityPtr> m_Entities;
    std::list<EntitySystem *> m_Systems;
    std::list<EntityModule *> m_Modules;

    Entity::TEntityPtr addEntity();

public:
    EntityContext();

    template<class T>
    void registerComponent(std::string name) {
        m_ComponentFactory.registerName<T>(name);
    }

    template<class T, typename = std::enable_if_t<std::is_base_of<EntitySystem, T>::value>>
    T *registerEntitySystem() {
        auto *p = new T();
        m_Systems.push_back(p);

        return p;
    }

    template<class T, typename = std::enable_if_t<std::is_base_of<EntityModule, T>::value>>
    T *registerModule() {
        auto *p = new T();
        m_Modules.push_back(p);
        m_Modules.back()->registerComponents(*this);
        m_Modules.back()->registerSystems(*this);

        return p;
    }

    template<class T, typename = std::enable_if_t<std::is_base_of<EntitySystem, T>::value>>
    EntitySystem *getSystem() {
        for (const auto &elem: m_Systems) {
            if (dynamic_cast<T *>(elem)) {
                return elem;
            }
        }

        return nullptr;
    }

    template<class T>
    void registerComponentWithEntitySystem(Component *component) {
        auto *system = getSystem<T>();
        if (system != nullptr) {
            system->registerComponent(component);
        }
    }

    void unregisterComponentFromSystems(Component *);
    void deserializeEntityMap(nlohmann::json &j);
    Entity::TEntityPtr createEntity(const std::string &configurationName, ResourceManager &);
    void deserializeEntities(nlohmann::json &j, ResourceManager &);
    void serializeEntities(nlohmann::json &j);
    void registerEntitiesWithSystems();
    void initializeSystems(ResourceManager*, EventManager *eventManager);
    void processSystems();
};
