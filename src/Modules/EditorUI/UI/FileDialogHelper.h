#pragma once
#include "../../../Helper/StringUtils.h"


#include "../../../SourceLibs/imgui/ImGuiFileDialog.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include <string>
#include <unistd.h>

inline bool FileInput(const std::string &name, const std::string &title, const std::string &ext, const std::string &label, std::string &path, const std::string& displayPath = "") {
    std::string buttonName = "Browse##" + name + "BrowseButton";
    if (ImGui::Button(buttonName.c_str())) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog(name, title, ext.c_str(), config);
    }
    ImGui::SameLine();
    if (displayPath.empty()) {
        ImGui::Text("%s: -", label.c_str());
    } else {
        ImGui::Text("%s: %s", label.c_str(), displayPath.c_str());
    }

    if (ImGuiFileDialog::Instance()->Display(name)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {

            path = ImGuiFileDialog::Instance()->GetFilePathName();

            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) {

                StringUtils::removeSubstring(path, std::string(cwd));
                path.erase(0, 1);

                // std::cout<<path<<std::endl;
            }
            ImGuiFileDialog::Instance()->Close();

            return true;
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    return false;
}