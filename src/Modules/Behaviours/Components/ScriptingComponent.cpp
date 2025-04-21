#include "ScriptingComponent.h"

ScriptingComponent::ScriptingComponent() : Component() {
}

void ScriptingComponent::serialize(nlohmann::json &j) {
     j[SCRIPT_FILE_KEY] = m_scriptFile;
     if (!m_scriptSource.empty()) {
          j[SCRIPT_SOURCE_KEY] = m_scriptSource;
     }
}

void ScriptingComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
     m_scriptFile = j.value(SCRIPT_FILE_KEY, m_scriptFile);
     m_scriptSource = j.value(SCRIPT_SOURCE_KEY, "");
}