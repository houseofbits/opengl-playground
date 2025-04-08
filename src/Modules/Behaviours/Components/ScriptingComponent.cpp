#include "ScriptingComponent.h"

ScriptingComponent::ScriptingComponent() : Component() {
}

void ScriptingComponent::serialize(nlohmann::json &j) {
     j[SCRIPT_FILE_KEY] = m_scriptFile;
}

void ScriptingComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
     m_scriptFile = j.value(SCRIPT_FILE_KEY, m_scriptFile);
}