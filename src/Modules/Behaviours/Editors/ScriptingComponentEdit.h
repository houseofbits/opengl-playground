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

    if (ImGui::Button("Reload file")) {
        system.m_EntityContext->scriptingManager.runScriptFromFile(component->m_scriptFile);
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove file")) {
        component->m_scriptFile = "";
    }

    ImGui::Spacing();

    if (ImGui::TreeNode("Source")) {
        static char text[1024 * 16];
        std::strncpy(text, component->m_scriptSource.c_str(), sizeof(text) - 2);
        text[sizeof(text) - 1] = '\0';

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        if (ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 30), flags)) {
            component->m_scriptSource = text;
        }

        if (ImGui::Button("Run")) {
            system.m_EntityContext->scriptingManager.runScript(component->m_scriptSource);
        }
        ImGui::TreePop();
    }
}
