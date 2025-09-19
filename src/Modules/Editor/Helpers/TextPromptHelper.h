#pragma once
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include <string>

class TextPromptHelper {
public:
    inline static std::string m_InputValue;

    static bool textPrompt(const std::string& buttonLabel, const std::string& title, const std::string& label) {
        static bool isPromptWindowVisible = false;

        if (ImGui::Button(buttonLabel.c_str())) {
            isPromptWindowVisible = true;
            m_InputValue = "";
        }

        if (isPromptWindowVisible && ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::PushItemWidth(200);
            ImGui::InputText(label.c_str(), &m_InputValue);
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::Separator();
            if (ImGui::Button("Close")) {
                isPromptWindowVisible = false;
                ImGui::End();
                return false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Submit")) {
                isPromptWindowVisible = false;
                ImGui::End();
                return true;
            }

            ImGui::End();
        }

        return false;
    }
};