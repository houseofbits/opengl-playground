#pragma once

#include "../../../../Core/Reflection/Identity.h"
#include "../../../../SourceLibs/imgui/imgui.h"
#include "../../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include <string>

class EditorUISystem;

class BaseComponentEdit {
public:
    explicit BaseComponentEdit(EditorUISystem *uiSystem) : m_EditorUISystem(uiSystem) {
    }

    virtual bool isEntityEditable(Identity::Type entityId) = 0;
    virtual std::string getName() = 0;
    virtual void process(Identity::Type entityId) = 0;

    EditorUISystem *m_EditorUISystem;
};