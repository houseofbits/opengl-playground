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

    std::list<std::shared_ptr<Entity>> &getAllEntities() {
        return m_Entities;
    }

    template<class T>
    void registerComponent(std::string alias = "") {
        m_ComponentFactory.registerName<T>(T::TypeName());
        if (!alias.empty()) {
            m_ComponentFactory.registerName<T>(alias);
        }
    }

    template<class T, typename = std::enable_if_t<std::is_base_of<EntitySystem, T>::value>>
    T *registerEntitySystem(unsigned int processPriority = 0) {
        auto *p = new T();
        p->m_EntityContext = this;
        p->m_processPriority = processPriority;
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

    template<class T, class A>
    void registerComponentWithEntitySystemHaving(Component *component) {
        auto *system = getSystem<T>();
        if (system == nullptr) {
            return;
        }
        if (!doesEntityHaveComponent<A>(component->m_EntityId.id())) {
            return;
        }

        system->registerComponent(component);
    }

    template<class T>
    bool doesEntityHaveComponent(Identity::Type id) {
        auto *e = getEntity(id);
        if (e != nullptr) {
            auto *c = e->getComponent<T>();

            return c != nullptr;
        }

        return false;
    }

    template<class T>
    T *getEntityComponent(Identity::Type id) {
        auto *e = getEntity(id);
        if (e) {
            return e->getComponent<T>();
        }

        return nullptr;
    }

    template<class T>
    T *findEntityComponent(const std::string &entityName) {
        auto *e = findEntity(entityName);
        if (e) {
            return e->getComponent<T>();
        }

        return nullptr;
    }

    Entity *getEntity(Identity::Type id);
    Entity *findEntity(const std::string &name);
    void unregisterComponentFromSystems(Component *);
    void deserializeEntityMap(nlohmann::json &j);
    std::shared_ptr<Entity> createEntityFromJson(nlohmann::json &j, ResourceManager &);
    std::shared_ptr<Entity> createEntityFromTemplate(const std::string &configurationName, ResourceManager &);
    void removeEntity(int entityId);
    void deserializeEntities(nlohmann::json &j, ResourceManager &);
    void serializeEntities(nlohmann::json &j);
    void registerEntitiesWithSystems();
    void initializeSystems(ResourceManager *, EventManager *eventManager);
    void processSystems();
};
