#pragma once

#include <sol/sol.hpp>
#include <map>
#include "../Events/Event.h"
#include "../Helper/Log.h"

class BaseEventCallback {
public:
    virtual ~BaseEventCallback() = default;
    virtual void setCallback(const sol::function &func) = 0;
    virtual void call(const BaseEvent &event) = 0;
};

template<class EventT>
class EventCallback final : public BaseEventCallback {
public:
    EventCallback() = default;

    void setCallback(const sol::function &func) override {
        m_callback = func;
    }

    void call(const BaseEvent &event) override {
        if (m_callback) {
            auto concreteEvent = static_cast<const EventT *>(&event);
            m_callback(*concreteEvent);
        }
    }

private:
    std::function<void(const EventT &)> m_callback;
};

class ScriptingManager {
public:
    ScriptingManager();

    void init();

    template<typename Class, typename... Args>
    void registerEventType(std::string classname, Args &&... args) {
        m_luaState.new_usertype<Class>(classname, std::forward<Args>(args)...);
        m_eventHandlers[classname] = new EventCallback<Class>();
    }

    void handleEvent(const std::string &name, const BaseEvent &event) const;

    void runScriptFromFile(const std::string &fileName);

private:
    void registerEventHandler(const std::string &eventName, const sol::function &func);

    sol::state m_luaState;
    std::map<std::string, BaseEventCallback *> m_eventHandlers;
};
