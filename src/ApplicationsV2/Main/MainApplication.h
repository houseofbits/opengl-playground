#pragma once

#include "../../CoreV2/API.h"
#include "../../ModulesV2/EditorUI/Events/EditorUIEvent.h"
#include "../../Window.h"

class MainApplication : public Application {
public:
    MainApplication();

    void run() override;
    void initialize(const std::string &entityDefinitionFileName);
    bool handleInputEvent(InputEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);
    bool handleEntityCreationEvent(EntityCreationEvent *event);

    Window m_Window;
};
