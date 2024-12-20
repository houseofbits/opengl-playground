#pragma once

#include "../../Core/API.h"
#include "../../Modules/Editor/Events/EditorUIEvent.h"
#include "../../Window.h"

class EditorApplication : public Application, public EventHandler {
public:
    EditorApplication();

    void run() override;

    void initialize(const std::string &entityDefinitionFileName);

    void handleInputEvent(const InputEvent&);

    void handleEditorUIEvent(const EditorUIEvent&);

    void handleEntityCreationEvent(const EntityCreationEvent&);

    //Used for event manager to identify target handlers
    Identity::Type getId() override { return 0; }

    Window m_Window;
};
