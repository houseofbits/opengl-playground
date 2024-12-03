#pragma once

#include "../../../Core/API.h"
#include "../../../Core/Reflection/Identity.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include <string>

class EditorUISystem;

class BaseComponentEdit {
public:
    virtual std::string getName() = 0;
    virtual void process(Entity &entity, EditorUISystem& system) = 0;
};