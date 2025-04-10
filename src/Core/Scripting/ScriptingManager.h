#pragma once

#include <sol/sol.hpp>
#include <map>
#include "../Events/Event.h"
#include "ScriptingEventCallback.h"
#include "../Entity/Component.h"

class BaseScriptingComponentBuilder {
public:
    virtual ~BaseScriptingComponentBuilder() = default;

    virtual sol::object make(sol::state &state, const Component &component) = 0;
};

template<class ComponentT>
class ScriptingComponentBuilder final : public BaseScriptingComponentBuilder {
public:
    sol::object make(sol::state &state, const Component &component) override {
        const auto comp = dynamic_cast<const ComponentT *>(&component);
        if (comp) return sol::make_object(state.lua_state(), comp);

        return sol::nil;
    }
};

class ScriptingManager {
public:
    explicit ScriptingManager(EntityContext &m_entityContext);

    void init();

    template<typename T, typename... Args>
    void registerEventType(std::string classname, Args &&... args) {
        m_luaState.new_usertype<T>(classname, std::forward<Args>(args)...);
        if (m_eventHandlers.find(classname) != m_eventHandlers.end()) {
            delete m_eventHandlers[classname];
        }
        m_eventHandlers[classname] = new ScriptingEventCallback<T>();
    }

    template<typename T, typename... Args>
    void registerComponentType(Args &&... args) {
        auto classname = T::TypeName();
        m_luaState.new_usertype<T>(classname, std::forward<Args>(args)...);
        if (m_componentBuilder.find(classname) != m_componentBuilder.end()) {
            delete m_componentBuilder[classname];
        }
        m_componentBuilder[classname] = new ScriptingComponentBuilder<T>();
    }

    void handleEvent(const std::string &name, const BaseEvent &event) const;

    void runScriptFromFile(const std::string &fileName);

private:
    void registerEventHandler(const std::string &eventName, const sol::function &func);

    sol::object getComponent(const std::string &entityName, const std::string &componentName);

    sol::state m_luaState;
    EntityContext *m_entityContext;
    std::map<std::string, BaseScriptingEventCallback *> m_eventHandlers;
    std::map<std::string, BaseScriptingComponentBuilder *> m_componentBuilder;
};
