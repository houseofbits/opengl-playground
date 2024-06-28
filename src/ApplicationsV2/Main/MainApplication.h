#pragma once

#include "../../CoreV2/API.h"
#include "../../CoreV2/Resources/ResourceManager.h"
#include "../../ModulesV2/EditorUI/Events/EditorUIEvent.h"
#include "../../Window.h"

class MainApplication : public Application {
public:
    MainApplication();

    void run() override;
    void initialize(const std::string &entityDefinitionFileName);
    bool handleInputEvent(InputEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);

    Window m_Window;
};
