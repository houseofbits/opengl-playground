#pragma once

#include <sol/sol.hpp>
#include "../Events/Event.h"

class BaseScriptingEventCallback {
public:
    virtual ~BaseScriptingEventCallback() = default;

    virtual void setCallback(const sol::function &func) = 0;

    virtual void call(const BaseEvent &event) = 0;
};

template<class EventT>
class ScriptingEventCallback final : public BaseScriptingEventCallback {
public:
    ScriptingEventCallback() = default;

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
