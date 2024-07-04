#pragma once

#include "../Module/EntityModule.h"
#include "../Reflection/Factory.h"
#include "Component.h"
#include "Entity.h"
#include "EntityConfiguration.h"
#include <memory>

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

    std::list<std::shared_ptr<Entity>> m_Entities;
    std::list<EntitySystem *> m_Systems;
    std::list<EntityModule *> m_Modules;

    std::shared_ptr<Entity> addEntity();

public:
    EntityContext();

    template<class T>
    void registerComponent(std::string name) {
        m_ComponentFactory.registerName<T>(name);
    }

    template<class T, typename = std::enable_if_t<std::is_base_of<EntitySystem, T>::value>>
    T *registerEntitySystem() {
        auto *p = new T();
        p->m_EntityContext = this;
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

    Entity* getEntity(Identity::Type id) ;

    template<class T>
    void registerComponentWithEntitySystem(Component *component) {
        auto *system = getSystem<T>();
        if (system != nullptr) {
            system->registerComponent(component);
        }
    }

    void unregisterComponentFromSystems(Component *);
    void deserializeEntityMap(nlohmann::json &j);
    std::shared_ptr<Entity> createEntity(const std::string &configurationName, ResourceManager &);
    void removeEntity(int entityId);
    void deserializeEntities(nlohmann::json &j, ResourceManager &);
    void serializeEntities(nlohmann::json &j);
    void registerEntitiesWithSystems();
    void initializeSystems(ResourceManager*, EventManager *eventManager);
    void processSystems();
};
