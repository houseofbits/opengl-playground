#include "ScriptingManager.h"
#include "../Helper/Log.h"
#include "../Entity/EntityContext.h"

ScriptingManager::ScriptingManager(EntityContext &m_entityContext) : m_entityContext(&m_entityContext) {
}

void ScriptingManager::init() {
    m_luaState.open_libraries(sol::lib::base, sol::lib::package);

    m_luaState.new_usertype<ScriptingManager>("ScriptingManager",
                                              "registerEventHandler", &ScriptingManager::registerEventHandler,
                                              "getComponent", &ScriptingManager::getComponent
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

void ScriptingManager::runScript(const std::string &source) {
    const sol::protected_function_result result = m_luaState.script(source);
    if (!result.valid()) {
        const sol::error err{result};
        Log::error("Error running script: ", err.what());
    }
}

void ScriptingManager::runScriptFromFile(const std::string &fileName) {
    sol::load_result script = m_luaState.load_file(fileName);
    if (!script.valid()) {
        const sol::error err{script};
        Log::error("Error loading file: ", err.what());

        return;
    }

    if (const sol::protected_function_result result = script(); !result.valid()) {
        const sol::error err{result};
        Log::error("Error running script: ", err.what());
    }
}

sol::object ScriptingManager::getComponent(const std::string& entityName, const std::string& componentName) {
    // sol::state_view lua = sol::state_view(m_luaState);

    // Log::write("Get ", entityName, " / ", componentName);

    const auto entity = m_entityContext->findEntity(entityName);
    if (!entity) {
        return sol::nil;
    }

    auto component = entity->findComponentByName(componentName);
    if (!component) {
        component = entity->findComponentByTypeName(componentName);
    }

    if (!component) {
        return sol::nil;
    }

    const auto builder = m_componentBuilder.find(component->getTypeName());
    if (builder == m_componentBuilder.end()) {
        return sol::nil;
    }

    return builder->second->make(m_luaState, *component);
}
