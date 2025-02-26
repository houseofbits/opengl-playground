#pragma once

#include "../Module/EntityModule.h"
#include "../Reflection/Factory.h"
#include "Component.h"
#include "Entity.h"
#include "EntityConfiguration.h"
#include "../System/EntitySystem.h"
#include "../System/EntitySystemRegistry.h"
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

    std::list<std::shared_ptr<Entity> > m_Entities;
    std::list<EntityModule *> m_Modules;

    std::shared_ptr<Entity> addEntity();

    std::map<EntitySystem *, EntitySystemRegistry::ProcessType> m_systemInitializers;

public:
    EntityContext();

    std::list<std::shared_ptr<Entity> > &getAllEntities() {
        return m_Entities;
    }

    template<class T>
    void registerComponent() {
        m_ComponentFactory.registerName<T>(T::TypeName());
    }

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntitySystem, T> > >
    void registerEntitySystem(EntitySystemRegistry::ProcessType processType, unsigned int processPriority = 0) {
        auto *p = new T();
        p->m_EntityContext = this;
        p->m_processPriority = processPriority;

        m_systemInitializers[p] = processType;
    }

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntityModule, T> > >
    T *registerModule() {
        auto *p = new T();
        m_Modules.push_back(p);
        m_Modules.back()->registerComponents(*this);
        m_Modules.back()->registerSystems(*this);

        return p;
    }

    void postRegisterModules() {
        for (const auto module: m_Modules) {
            module->postRegister(*this);
        }
    }

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntitySystem, T> > >
    T *getSystem() {
        return entitySystemRegistry.getSystem<T>();
    }

    template<class T>
    bool doesEntityHaveComponent(const Identity::Type entityId) {
        auto *e = getEntity(entityId);
        if (e != nullptr) {
            auto *c = e->getComponent<T>();

            return c != nullptr;
        }

        return false;
    }

    template<class T>
    T *getEntityComponent(const Identity::Type entityId) {
        auto *e = getEntity(entityId);
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

    template<class T>
    T *getComponent(const Identity::Type componentId) {
        for (const auto &e: m_Entities) {
            if (auto component = e->getComponent(componentId)) {
                auto cT = dynamic_cast<T *>(component);
                if (cT) {
                    return cT;
                }

                return nullptr;
            }
        }
        return nullptr;
    }

    Entity *getEntity(Identity::Type id);

    Entity *findEntity(const std::string &name);

    Entity *findEntity(std::function<bool(Entity *)> functor);

    void unregisterEntityFromSystems(Entity &);

    void deserializeEntityMap(nlohmann::json &j);

    std::shared_ptr<Entity> createEntityFromJson(nlohmann::json &j);

    std::shared_ptr<Entity> createEntityFromTemplate(const std::string &configurationName, ResourceManager &);

    void removeEntity(int entityId);

    void deserializeEntities(nlohmann::json &j, ResourceManager &);

    void serializeEntities(nlohmann::json &j) const;

    void initializeEntities(EventManager &eventManager);

    void registerEntityWithSystems(Entity &entity) const; //TODO: Private ???

    void initializeSystems(ResourceManager &, EventManager &);

    void processSystems(EventManager &eventManager);

    void createComponentInplace(Identity::Type entityId, const std::string &componentName);

    void removeComponent(Identity::Type entityId, const std::string &componentName);

    std::vector<std::string> getAllConfigurationNames();

    EntitySystemRegistry entitySystemRegistry;
};
