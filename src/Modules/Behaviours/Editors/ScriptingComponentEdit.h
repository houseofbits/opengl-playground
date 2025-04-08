#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/ScriptingComponent.h"

inline void processScriptingComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<ScriptingComponent *>(c);

    static std::string m_filePath;

    if (FileInput("ChooseScriptFile", "Choose LUA Script file", ".lua", "Script", m_filePath,
                  component->m_scriptFile)) {
        component->m_scriptFile = m_filePath;
        system.m_EntityContext->scriptingManager.runScriptFromFile(m_filePath);
    }

    if (ImGui::Button("Reload")) {
        system.m_EntityContext->scriptingManager.runScriptFromFile(component->m_scriptFile);
    }
}
