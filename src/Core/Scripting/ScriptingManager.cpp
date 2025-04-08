#include "ScriptingManager.h"
#include "../Helper/Log.h"

ScriptingManager::ScriptingManager()
= default;

void ScriptingManager::init() {
    m_luaState.open_libraries(sol::lib::base, sol::lib::package);

    m_luaState.new_usertype<ScriptingManager>("ScriptingManager",
                                              "registerEventHandler", &ScriptingManager::registerEventHandler
    );

    m_luaState["Manager"] = this;
}

void ScriptingManager::registerEventHandler(const std::string &eventName, const sol::function &func) {
    if (m_eventHandlers.find(eventName) == m_eventHandlers.end()) {
        Log::warn("ScriptingManager: ", eventName + " is not registered");

        return;
    }

    m_eventHandlers.at(eventName)->setCallback(func);
}

void ScriptingManager::handleEvent(const std::string &name, const BaseEvent &event) const {
    m_eventHandlers.at(name)->call(event);
}

void ScriptingManager::runScriptFromFile(const std::string &fileName) {
    sol::load_result script = m_luaState.load_file(fileName);
    if (!script.valid()) {
        const sol::error err{script};
        Log::error("Error loading file: ", err.what());

        return;
    }

    sol::protected_function_result result = script();
    if (!result.valid()) {
        const sol::error err{result};
        Log::error("Error running script: ", err.what());
    }
}
