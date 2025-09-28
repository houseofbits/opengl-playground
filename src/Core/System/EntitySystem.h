#pragma once

#include "../Entity/Component.h"
#include "../Reflection/Type.h"
// #include "../Events/EventManager.h"
#include "../Events/EventManager_V2.h"
#include "AbstractComponentRegistry.h"
#include "EntityRelatedComponentRegistry.h"
#include "EntityUniqueComponentRegistry.h"
#include "SameComponentRegistry.h"
#include <memory>
#include "../Entity/EntityContext.h"
#include "../Helper/Profilable.h"

class EventManager;
class EntityContext;
class ResourceManager;
class Entity;

class EntitySystem : public EventListener, public Profilable {
public:
    EntitySystem() : EventListener() {
    }

    typedef std::shared_ptr<EntitySystem> TEntitySystemPtr;

    /**
     * Initialize is run for all systems once on startup. After the render context
     * is created and before entities are deserialized
     */
    virtual void initialize(ResourceManager &, EventManager&) {
    };
    /**
     * Process runs each frame for each system
     */
    virtual void process(EventManager &) {
    };

    virtual void registerEventHandlers(EventManager &) {
    }

    //Used for event manager to identify target handlers
    Identity::Type getId() override {
        return 0;
    }

    EntityContext *m_EntityContext{nullptr};
    EventManager *m_EventManager{nullptr};
    unsigned int m_processPriority{};

    /**
     * @details Handles multiple and unique entity components indexed by entity id
     */
    template<class... Ts>
    EntityRelatedComponentRegistry<Ts...> *useEntityRelatedComponentsRegistry() {
        auto reg = new EntityRelatedComponentRegistry<Ts...>();
        m_registries.push_back(reg);

        return reg;
    }

    /**
     * @details Handles single unique entity component indexed by entity id
     */
    template<class T>
    EntityUniqueComponentRegistry<T> *useEntityUniqueComponentRegistry() {
        auto reg = new EntityUniqueComponentRegistry<T>();
        m_registries.push_back(reg);

        return reg;
    }

    /**
     * @details Contains non unique entity components indexed by component id
     */
    template<class T>
    SameComponentRegistry<T> *useSameComponentRegistry() {
        auto reg = new SameComponentRegistry<T>();
        m_registries.push_back(reg);

        return reg;
    }

    virtual void registerEntityComponents(Entity &entity) {
        for (const auto &reg: m_registries) {
            reg->registerComponents(entity);
        }
    }

    virtual void unregisterEntityComponents(Entity &entity) {
        for (const auto &reg: m_registries) {
            reg->unregisterComponents(entity);
        }
    }

    std::list<AbstractComponentRegistry *> m_registries;
    std::string m_name;
};
